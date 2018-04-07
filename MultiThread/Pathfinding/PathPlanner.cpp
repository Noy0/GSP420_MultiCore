#include "PathPlanner.h"
#include "Common\Graph\HandyGraphFunctions.h"
#include "constants.h"
#include "Common\Graph\AStarHeuristicPolicies.h"
#include "Common\misc\Stream_Utility_Functions.h"


#include <iostream>
using namespace std;

//----------------------- CreateGraph ------------------------------------
//
//------------------------------------------------------------------------
void PathPlanner::CreateGraph(int CellsUp, int CellsAcross)
{
  //initialize the terrain vector with normal terrain
  m_TerrainType.assign(CellsUp * CellsAcross, normal);

  m_iCellsX     = CellsAcross;
  m_iCellsY     = CellsUp;
  m_dCellWidth  = (double)m_icxClient / (double)CellsAcross;
  m_dCellHeight = (double)m_icyClient / (double)CellsUp;

  //delete any old graph
  delete m_pGraph;

  //create the graph
  m_pGraph = new NavGraph(false);//not a digraph

  GraphHelper_CreateGrid(*m_pGraph, m_icxClient, m_icyClient, CellsUp, CellsAcross);

  //initialize source and target indexes to mid top and bottom of grid 
  PointToIndex(VectorToPOINTS(Vector2D(m_icxClient/2, m_dCellHeight*2)), m_iTargetCell);
  PointToIndex(VectorToPOINTS(Vector2D(m_icxClient/2, m_icyClient -m_dCellHeight*2)), m_iSourceCell);

  m_Path.clear();
  
  m_CurrentAlgorithm = search_astar;
}

//--------------------- PointToIndex -------------------------------------
//
//  converts a POINTS into an index into the graph
//------------------------------------------------------------------------
bool PathPlanner::PointToIndex(POINTS p, int& NodeIndex)
{
  //convert p to an index into the graph
  int x = (int)((double)(p.x)/m_dCellWidth);  
  int y = (int)((double)(p.y)/m_dCellHeight); 
  
  //make sure the values are legal
  if ( (x>m_iCellsX) || (y>m_iCellsY) )
  {
    NodeIndex = -1;

    return false;
  }

  NodeIndex = y*m_iCellsX+x;

  return true;
}

//----------------- GetTerrainCost ---------------------------------------
//
//  returns the cost of the terrain represented by the current brush type
//------------------------------------------------------------------------
double PathPlanner::GetTerrainCost(const brush_type brush)
{
  const double cost_normal = 1.0;
  //const double cost_water  = 2.0;
  //const double cost_mud    = 1.5;

  switch (brush)
  {
    case normal: return cost_normal;
    //case water:  return cost_water;
    //case mud:    return cost_mud;
    default:     return MaxDouble;
  };
}

//---------------------TrimGraphForCircle----------------------------
//
//This method will take the square graph and make invalid all nodes
//outside the radius of the circle
//
//-------------------------------------------------------------------
void PathPlanner::TrimGraphForCircle()
{
	double radius = GetRadius();
	for(int i=0; i<m_pGraph->NumNodes(); ++i)
	{
		NavGraphNode<void*> ngn = m_pGraph->GetNode(i);
		Vector2D pos = ngn.Pos();
		
		double radDist = radius*radius;
		double testDist = (200-pos.x)*(200-pos.x) + (200-pos.y)*(200-pos.y);

		if(testDist > radDist)
		{
			UpdateGraphFromBrush(1, i);
		}
	}
}

//--------------------------- UpdateGraphFromBrush ----------------------------
//
//  given a brush and a node index, this method updates the graph appropriately
//  (by removing/adding nodes or changing the costs of the node's edges)
//-----------------------------------------------------------------------------
void PathPlanner::UpdateGraphFromBrush(int brush, int CellIndex)
{
  //set the terrain type in the terrain index
  m_TerrainType[CellIndex] = brush;

  //if current brush is an obstacle then this node must be removed
  //from the graph
  if (brush == 1)
  {
    m_pGraph->RemoveNode(CellIndex);
  }

  else
  {
    //make the node active again if it is currently inactive
    if (!m_pGraph->isNodePresent(CellIndex))
    {
      int y = CellIndex / m_iCellsY;
      int x = CellIndex - (y*m_iCellsY);

      m_pGraph->AddNode(NavGraph::NodeType(CellIndex, Vector2D(x*m_dCellWidth + m_dCellWidth/2.0,
                                                               y*m_dCellHeight+m_dCellHeight/2.0)));

      GraphHelper_AddAllNeighboursToGridNode(*m_pGraph, y, x, m_iCellsX, m_iCellsY);
    }

    //set the edge costs in the graph
    WeightNavGraphNodeEdges(*m_pGraph, CellIndex, GetTerrainCost((brush_type)brush));                            
  }
}

//--------------------------- CreatePathAStar ---------------------------
//------------------------------------------------------------------------
void PathPlanner::CreatePathAStar()
{
  //set current algorithm
  m_CurrentAlgorithm = search_astar;
  
  //create a couple of typedefs so the code will sit comfortably on the page   
  typedef Graph_SearchAStar<NavGraph, Heuristic_Euclid> AStarSearch;

  //create an instance of the A* search using the Euclidean heuristic
  AStarSearch AStar(*m_pGraph, m_iSourceCell, m_iTargetCell);
  

  //record the time taken  
  //m_dTimeTaken = timer.TimeElapsed();

  m_Path = AStar.GetPathToTarget();

  m_dCostToTarget = AStar.GetCostToTarget();

}

//---------------------------Load n save methods ------------------------------
//-----------------------------------------------------------------------------
void PathPlanner::Save( char* FileName)
{
  ofstream save(FileName);
  assert (save && "PathPlanner::Save< bad file >");

  //save the size of the grid
  save << m_iCellsX << endl;
  save << m_iCellsY << endl;

  //save the terrain
  for (unsigned int t=0; t<m_TerrainType.size(); ++t)
  {
    if (t==m_iSourceCell)
    {
      save << source << endl;
    }
    else if (t==m_iTargetCell)
    {
      save << target << endl;
    }
    else
    {
      save << m_TerrainType[t] << endl;
    }
  }
}

//-------------------------------- Load ---------------------------------------
//-----------------------------------------------------------------------------
void PathPlanner::Load( char* FileName)
{
  ifstream load(FileName);
  assert (load && "PathPlanner::Save< bad file >");

  //load the size of the grid
  load >> m_iCellsX;
  load >> m_iCellsY;

  //create a graph of the correct size
  CreateGraph(m_iCellsY, m_iCellsX);

  int terrain;

  //save the terrain
  for (int t=0; t<m_iCellsX*m_iCellsY; ++t)
  {
    load >> terrain;
    
    if (terrain == source)
    {
      m_iSourceCell = t;
    }

    else if (terrain == target)
    {
      m_iTargetCell = t;
    }

    else
    {
      m_TerrainType[t] = terrain;

      UpdateGraphFromBrush(terrain, t);
    }
  }
}