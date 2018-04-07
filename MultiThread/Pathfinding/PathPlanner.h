#ifndef PATHPLANNER_H
#define PATHPLANNER_H
#pragma warning (disable:4786)
//------------------------------------------------------------------------
//
//  Name:   PathPlanner.h
//
//  Desc:   class enabling users to create simple environments consisting
//          of different terrain types and then to use various search algorithms
//          to find paths through them
//
//  Author: Mat Buckland  (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include <windows.h>
#include <vector>
#include <fstream>
#include <string>
#include <list>


#include "Common\2D\Vector2d.h"
#include "Common\Graph\SparseGraph.h"
#include "Common\Graph\GraphAlgorithms.h"
#include "Common\Misc\utils.h"
#include "Common\Graph\GraphEdgeTypes.h"
#include "Common\Graph\GraphNodeTypes.h"

class PathPlanner
{
public:

  enum brush_type
  {
    normal   = 0,
    source   = 4,
    target   = 5
  };
      
  enum algorithm_type
  {
    search_astar
  };

  //for ease of use typdef the graph edge/node types used by the navgraph
  typedef std::list<int>				Path;

   //create a typedef for the graph type
  typedef SparseGraph<NavGraphNode<void*>, GraphEdge> NavGraph;

  NavGraph*                     m_pGraph;

private:
  
  //the terrain type of each cell
  std::vector<int>              m_TerrainType;

  //this vector will store any path returned from a graph search
  Path							m_Path;

  //the total cost of the path from target to source
  double                        m_dCostToTarget;

  //the currently selected algorithm
  algorithm_type                m_CurrentAlgorithm;

  //the current terrain brush
  brush_type                    m_CurrentTerrainBrush;

  //the dimensions of the cells
  double                        m_dCellWidth;
  double                        m_dCellHeight;

  //number of cells vertically and horizontally
  int                           m_iCellsX,
                                m_iCellsY;

  //radius of table
  int							m_iRadius;

  //local record of the client area
  int                           m_icxClient,
                                m_icyClient;

  //the indices of the source and target cells
  int                           m_iSourceCell,
                                m_iTargetCell;

  //flags to indicate if the start and finish points have been added
  bool                          m_bStart,
                                m_bFinish;
  
  //this calls the appropriate algorithm
  void  UpdateAlgorithm();

public:

  PathPlanner(int pRadius = 200):m_bStart(false),
                m_bFinish(false),
                m_dCellWidth(0),
                m_dCellHeight(0),
                m_iCellsX(0),
                m_iCellsY(0),
                m_CurrentTerrainBrush(normal),
                m_iSourceCell(0),
                m_iTargetCell(0),
				m_iRadius(pRadius),
                m_dCostToTarget(0.0),
                m_pGraph(NULL)
  {
	  if (pRadius <= 0) {m_iRadius = 200;}
	  m_icxClient = 2*m_iRadius;
	  m_icyClient = m_icxClient;
  }

  ~PathPlanner(){delete m_pGraph;}

  void CreateGraph(int CellsUp, int CellsAcross);

  //the algorithms
  void CreatePathAStar();

  void ChangeBrush(const brush_type NewBrush){m_CurrentTerrainBrush = NewBrush;}

  void ChangeSource(const int cell){m_iSourceCell = cell;}
  void ChangeTarget(const int cell){m_iTargetCell = cell;}

  //get path created by aStar search
  Path GetPath() {return m_Path;}

  double GetRadius() {return m_iRadius;}

  NavGraph * GetGraph() {return m_pGraph;}

  //truncate graph for circular grid
  void TrimGraphForCircle();
	
  double GetCostToTarget() {return m_dCostToTarget;}
	
  void  UpdateGraphFromBrush(int brush, int CellIndex);

  //converts a POINTS to an index into the graph. Returns false if p
  //is invalid
  bool PointToIndex(POINTS p, int& NodeIndex);

  //returns the terrain cost of the brush type
  double GetTerrainCost(brush_type brush);

  void Save( char* FileName);
  void Load( char* FileName);

};


#endif