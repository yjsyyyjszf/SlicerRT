/*==============================================================================

  Copyright (c) Radiation Medicine Program, University Health Network,
  Princess Margaret Hospital, Toronto, ON, Canada. All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Kevin Wang, Princess Margaret Cancer Centre 
  and was supported by Cancer Care Ontario (CCO)'s ACRU program 
  with funds provided by the Ontario Ministry of Health and Long-Term Care
  and Ontario Consortium for Adaptive Interventions in Radiation Oncology (OCAIRO).

==============================================================================*/

// Beams includes
#include "vtkMRMLRTBeamNode.h"
#include "vtkMRMLRTPlanNode.h"

// MRML includes
#include <vtkMRMLModelDisplayNode.h>
#include <vtkMRMLScalarVolumeNode.h>
#include <vtkMRMLDoubleArrayNode.h>
#include <vtkMRMLMarkupsFiducialNode.h>
#include <vtkMRMLLinearTransformNode.h>
#include <vtkMRMLSubjectHierarchyNode.h>
#include <vtkMRMLSubjectHierarchyConstants.h>

// VTK includes
#include <vtkObjectFactory.h>
#include <vtkSmartPointer.h>
#include <vtkIntArray.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkDoubleArray.h>
#include <vtkCellArray.h>

// SlicerRt includes
#include "PlmCommon.h"

//------------------------------------------------------------------------------
const char* vtkMRMLRTBeamNode::NEW_BEAM_NODE_NAME_PREFIX = "NewBeam_";

//------------------------------------------------------------------------------
static const char* MLCPOSITION_REFERENCE_ROLE = "MLCPositionRef";
static const char* DRR_REFERENCE_ROLE = "DRRRef";
static const char* CONTOUR_BEV_REFERENCE_ROLE = "contourBEVRef";

//------------------------------------------------------------------------------
vtkMRMLNodeNewMacro(vtkMRMLRTBeamNode);

//----------------------------------------------------------------------------
vtkMRMLRTBeamNode::vtkMRMLRTBeamNode()
{
  this->BeamNumber = -1;
  this->BeamDescription = NULL;

  this->BeamType = vtkMRMLRTBeamNode::Static;
  this->RadiationType = vtkMRMLRTBeamNode::Proton;
  this->CollimatorType = vtkMRMLRTBeamNode::SquareHalfMM;

  this->X1Jaw = -100.0;
  this->X2Jaw = 100.0;
  this->Y1Jaw = -100.0;
  this->Y2Jaw = 100.0;

  this->GantryAngle = 0.0;
  this->CollimatorAngle = 0.0;
  this->CouchAngle = 0.0;

  this->SAD = 2000.0;
  this->BeamWeight = 1.0;

  // Register parent transform modified event
  //TODO: Needed?
  vtkSmartPointer<vtkIntArray> events = vtkSmartPointer<vtkIntArray>::New();
  events->InsertNextValue(vtkMRMLTransformableNode::TransformModifiedEvent);
  vtkObserveMRMLObjectEventsMacro(this, events);
}

//----------------------------------------------------------------------------
vtkMRMLRTBeamNode::~vtkMRMLRTBeamNode()
{
  this->SetBeamDescription(NULL);
}

//----------------------------------------------------------------------------
void vtkMRMLRTBeamNode::WriteXML(ostream& of, int nIndent)
{
  Superclass::WriteXML(of, nIndent);

  // Write all MRML node attributes into output stream
  vtkIndent indent(nIndent);

  //TODO: Beam parameters
}

//----------------------------------------------------------------------------
void vtkMRMLRTBeamNode::ReadXMLAttributes(const char** atts)
{
  vtkMRMLNode::ReadXMLAttributes(atts);

  // Read all MRML node attributes from two arrays of names and values
  const char* attName = NULL;
  const char* attValue = NULL;

  while (*atts != NULL) 
  {
    attName = *(atts++);
    attValue = *(atts++);

    //if (!strcmp(attName, "TargetSegmentID")) 
    //{
    //  //this->NextBeamNumber = vtkVariant(attValue).ToDouble();
    //  this->SetTargetSegmentID(attValue);
    //}

    //TODO: Beam parameters
  }
}

//----------------------------------------------------------------------------
// Copy the node's attributes to this object.
// Does NOT copy: ID, FilePrefix, Name, VolumeID
void vtkMRMLRTBeamNode::Copy(vtkMRMLNode *anode)
{
  Superclass::Copy(anode);

  vtkMRMLRTBeamNode* node = vtkMRMLRTBeamNode::SafeDownCast(anode);
  if (!node)
  {
    return;
  }

  this->DisableModifiedEventOn();

  //TODO: Beam parameters
  this->SetSAD(node->GetSAD());

  //TODO: Beam parameters

  this->DisableModifiedEventOff();
  this->InvokePendingModifiedEvent();
}

//----------------------------------------------------------------------------
void vtkMRMLRTBeamNode::PrintSelf(ostream& os, vtkIndent indent)
{
  Superclass::PrintSelf(os,indent);

  //TODO: Beam parameters
}

//----------------------------------------------------------------------------
vtkMRMLDoubleArrayNode* vtkMRMLRTBeamNode::GetMLCPositionDoubleArrayNode()
{
  return vtkMRMLDoubleArrayNode::SafeDownCast( this->GetNodeReference(MLCPOSITION_REFERENCE_ROLE) );
}

//----------------------------------------------------------------------------
void vtkMRMLRTBeamNode::SetAndObserveMLCPositionDoubleArrayNode(vtkMRMLDoubleArrayNode* node)
{
  this->SetNodeReferenceID(MLCPOSITION_REFERENCE_ROLE, (node ? node->GetID() : NULL));

  this->InvokeCustomModifiedEvent(vtkMRMLRTBeamNode::BeamGeometryModified);
}

//----------------------------------------------------------------------------
vtkMRMLScalarVolumeNode* vtkMRMLRTBeamNode::GetDRRVolumeNode()
{
  return vtkMRMLScalarVolumeNode::SafeDownCast( this->GetNodeReference(DRR_REFERENCE_ROLE) );
}

//----------------------------------------------------------------------------
void vtkMRMLRTBeamNode::SetAndObserveDRRVolumeNode(vtkMRMLScalarVolumeNode* node)
{
  this->SetNodeReferenceID(DRR_REFERENCE_ROLE, (node ? node->GetID() : NULL));
}

//----------------------------------------------------------------------------
vtkMRMLScalarVolumeNode* vtkMRMLRTBeamNode::GetContourBEVVolumeNode()
{
  return vtkMRMLScalarVolumeNode::SafeDownCast( this->GetNodeReference(CONTOUR_BEV_REFERENCE_ROLE) );
}

//----------------------------------------------------------------------------
void vtkMRMLRTBeamNode::SetAndObserveContourBEVVolumeNode(vtkMRMLScalarVolumeNode* node)
{
  this->SetNodeReferenceID(CONTOUR_BEV_REFERENCE_ROLE, (node ? node->GetID() : NULL));
}

//----------------------------------------------------------------------------
vtkMRMLRTPlanNode* vtkMRMLRTBeamNode::GetParentPlanNode()
{
  vtkMRMLSubjectHierarchyNode* beamShNode = vtkMRMLSubjectHierarchyNode::GetAssociatedSubjectHierarchyNode(this);
  if (!beamShNode)
  {
    vtkErrorMacro("GetRTPlanNode: Subject hierarchy must be enabled for this operation!");
    return NULL;
  }

  if (!beamShNode->GetParentNode())
  {
    vtkErrorMacro("GetRTPlanNode: Beam node " << this->Name << " does not have a subject hierarchy parent, so cannot access RT plan!");
    return NULL;
  }

  vtkMRMLSubjectHierarchyNode* planShNode = vtkMRMLSubjectHierarchyNode::SafeDownCast(beamShNode->GetParentNode());
  return vtkMRMLRTPlanNode::SafeDownCast(planShNode->GetAssociatedNode());
}

//----------------------------------------------------------------------------
bool vtkMRMLRTBeamNode::GetPlanIsocenterPosition(double isocenter[3])
{
  vtkMRMLRTPlanNode* parentPlanNode = this->GetParentPlanNode();
  if (!parentPlanNode)
  {
    vtkErrorMacro("GetPlanIsocenterPosition: Failed to access parent plan node");
    return false;
  }
  vtkMRMLMarkupsFiducialNode* poisMarkupsNode = parentPlanNode->GetPoisMarkupsFiducialNode();
  if (!poisMarkupsNode)
  {
    vtkErrorMacro("GetPlanIsocenterPosition: Failed to access POIs markups node");
    return false;
  }

  poisMarkupsNode->GetNthFiducialPosition(vtkMRMLRTPlanNode::ISOCENTER_FIDUCIAL_INDEX, isocenter);
  return true;
}

//----------------------------------------------------------------------------
bool vtkMRMLRTBeamNode::CalculateSourcePosition(double source[3])
{
  double isocenter[3] = {0.0,0.0,0.0};
  if (!this->GetPlanIsocenterPosition(isocenter))
  {
    vtkErrorMacro("CalculateSourcePosition: Failed to get plan isocenter position");
    return false;
  }

  double gantryAngleRadian = this->GantryAngle * M_PI / 180.0;

  source[0] = isocenter[0] + this->SAD * sin(gantryAngleRadian);
  source[1] = isocenter[1] - this->SAD * cos(gantryAngleRadian);
  source[2] = isocenter[2];

  return true;
}

//----------------------------------------------------------------------------
void vtkMRMLRTBeamNode::SetX1Jaw(double x1Jaw)
{
  this->X1Jaw = x1Jaw;
  this->Modified();
  this->InvokeCustomModifiedEvent(vtkMRMLRTBeamNode::BeamGeometryModified);
}

//----------------------------------------------------------------------------
void vtkMRMLRTBeamNode::SetX2Jaw(double x2Jaw)
{
  this->X2Jaw = x2Jaw;
  this->Modified();
  this->InvokeCustomModifiedEvent(vtkMRMLRTBeamNode::BeamGeometryModified);
}

//----------------------------------------------------------------------------
void vtkMRMLRTBeamNode::SetY1Jaw(double y1Jaw)
{
  this->Y1Jaw = y1Jaw;
  this->Modified();
  this->InvokeCustomModifiedEvent(vtkMRMLRTBeamNode::BeamGeometryModified);
}

//----------------------------------------------------------------------------
void vtkMRMLRTBeamNode::SetY2Jaw(double y2Jaw)
{
  this->Y2Jaw = y2Jaw;
  this->Modified();
  this->InvokeCustomModifiedEvent(vtkMRMLRTBeamNode::BeamGeometryModified);
}

//----------------------------------------------------------------------------
void vtkMRMLRTBeamNode::SetGantryAngle(double angle)
{
  this->GantryAngle = angle;
  this->Modified();
  this->InvokeCustomModifiedEvent(vtkMRMLRTBeamNode::BeamTransformModified);
}

//----------------------------------------------------------------------------
void vtkMRMLRTBeamNode::SetCollimatorAngle(double angle)
{
  this->CollimatorAngle = angle;
  this->Modified();
  this->InvokeCustomModifiedEvent(vtkMRMLRTBeamNode::BeamTransformModified);
}

//----------------------------------------------------------------------------
void vtkMRMLRTBeamNode::SetCouchAngle(double angle)
{
  this->CouchAngle = angle;
  this->Modified();
  this->InvokeCustomModifiedEvent(vtkMRMLRTBeamNode::BeamTransformModified);
}

//----------------------------------------------------------------------------
void vtkMRMLRTBeamNode::SetSAD(double sad)
{
  this->SAD = sad;
  this->Modified();
  this->InvokeCustomModifiedEvent(vtkMRMLRTBeamNode::BeamGeometryModified);
}

//----------------------------------------------------------------------------
void vtkMRMLRTBeamNode::CreateDefaultDisplayNodes()
{
  //TODO: Create display node here when splitting CreateDefaultBeamModel
  Superclass::CreateDefaultDisplayNodes();
}

//---------------------------------------------------------------------------
void vtkMRMLRTBeamNode::CreateDefaultBeamModel()
{
  if (!this->GetScene())
  {
    vtkErrorMacro ("CreateDefaultBeamModel: Invalid MRML scene");
    return;
  }

  //TODO: Split this function into CreateDefaultDisplayNodes, CreateBeamTransformNodes, etc.
  // and call them when MRML scene is set (make sure transforms are not created twice)
  // Create beam model
  vtkSmartPointer<vtkPolyData> beamModelPolyData = vtkSmartPointer<vtkPolyData>::New();
  this->CreateBeamPolyData(beamModelPolyData);

  // Transform for visualization
  //TODO: Awful names for transforms. They should be barToFooTransform
  vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
  transform->Identity();
  transform->RotateZ(0);
  transform->RotateY(0);
  transform->RotateX(-90);

  vtkSmartPointer<vtkTransform> transform2 = vtkSmartPointer<vtkTransform>::New();
  transform2->Identity();
  transform2->Translate(0.0, 0.0, 0.0);

  transform->PostMultiply();
  transform->Concatenate(transform2->GetMatrix());

  // Create transform node for beam
  vtkSmartPointer<vtkMRMLLinearTransformNode> transformNode = vtkSmartPointer<vtkMRMLLinearTransformNode>::New();
  std::string transformName = std::string(this->GetName()) + "_Transform";
  transformNode->SetName(transformName.c_str());
  this->GetScene()->AddNode(transformNode);
  transformNode->SetMatrixTransformToParent(transform->GetMatrix());
  transformNode->SetAttribute(vtkMRMLSubjectHierarchyConstants::GetSubjectHierarchyExcludeFromTreeAttributeName().c_str(), "1");

  // Create display node for beam
  vtkSmartPointer<vtkMRMLModelDisplayNode> beamModelDisplayNode = vtkSmartPointer<vtkMRMLModelDisplayNode>::New();
  beamModelDisplayNode = vtkMRMLModelDisplayNode::SafeDownCast(this->GetScene()->AddNode(beamModelDisplayNode));
  beamModelDisplayNode->SetColor(0.0, 1.0, 0.2);
  beamModelDisplayNode->SetOpacity(0.3);
  beamModelDisplayNode->SetBackfaceCulling(0); // Disable backface culling to make the back side of the contour visible as well
  beamModelDisplayNode->VisibilityOn(); 
  beamModelDisplayNode->SliceIntersectionVisibilityOn();

  // Setup beam model node
  this->SetAndObservePolyData(beamModelPolyData);
  this->SetAndObserveTransformNodeID(transformNode->GetID());
  this->SetAndObserveDisplayNodeID(beamModelDisplayNode->GetID());
}

//---------------------------------------------------------------------------
void vtkMRMLRTBeamNode::CreateBeamPolyData(vtkPolyData* beamModelPolyData)
{
  if (!beamModelPolyData)
  {
    vtkErrorMacro("CreateBeamPolyData: Invalid beam node");
    return;
  }

  vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
  vtkSmartPointer<vtkCellArray> cellArray = vtkSmartPointer<vtkCellArray>::New();

  vtkMRMLDoubleArrayNode* mlcArrayNode = this->GetMLCPositionDoubleArrayNode();
  if (mlcArrayNode)
  {
    // First we extract the shape of the MLC
    int x1count = this->X1Jaw/10.0;
    int x2count = this->X2Jaw/10.0;
    int numLeavesVisible = x2count - x1count; // Calculate the number of leaves visible
    int numPointsEachSide = numLeavesVisible *2;

    double y2LeafPosition[40];
    double y1LeafPosition[40];

    // Calculate Y2 first
    for (int i = x2count; i >= x1count; i--)
    {
      double leafPosition = mlcArrayNode->GetArray()->GetComponent(-(i-20), 1);
      if (leafPosition < this->Y2Jaw)
      {
        y2LeafPosition[-(i-20)] = leafPosition;
      }
      else
      {
        y2LeafPosition[-(i-20)] = this->Y2Jaw;
      }
    }
    // Calculate Y1 next
    for (int i = x2count; i >= x1count; i--)
    {
      double leafPosition = mlcArrayNode->GetArray()->GetComponent(-(i-20), 0);
      if (leafPosition < this->Y1Jaw)
      {
        y1LeafPosition[-(i-20)] = leafPosition;
      }
      else
      {
        y1LeafPosition[-(i-20)] = this->Y1Jaw;
      }
    }

    // Create beam model
    points->InsertPoint(0,0,0,this->SAD);

    int count = 1;
    for (int i = x2count; i > x1count; i--)
    {
      points->InsertPoint(count,-y2LeafPosition[-(i-20)]*2, i*10*2, -this->SAD );
      count ++;
      points->InsertPoint(count,-y2LeafPosition[-(i-20)]*2, (i-1)*10*2, -this->SAD );
      count ++;
    }

    for (int i = x1count; i < x2count; i++)
    {
      points->InsertPoint(count,y1LeafPosition[-(i-20)]*2, i*10*2, -this->SAD );
      count ++;
      points->InsertPoint(count,y1LeafPosition[-(i-20)]*2, (i+1)*10*2, -this->SAD );
      count ++;
    }

    for (int i = 1; i <numPointsEachSide; i++)
    {
      cellArray->InsertNextCell(3);
      cellArray->InsertCellPoint(0);
      cellArray->InsertCellPoint(i);
      cellArray->InsertCellPoint(i+1);
    }
    // Add connection between Y2 and Y1
    cellArray->InsertNextCell(3);
    cellArray->InsertCellPoint(0);
    cellArray->InsertCellPoint(numPointsEachSide);
    cellArray->InsertCellPoint(numPointsEachSide+1);
    for (int i = numPointsEachSide+1; i <2*numPointsEachSide; i++)
    {
      cellArray->InsertNextCell(3);
      cellArray->InsertCellPoint(0);
      cellArray->InsertCellPoint(i);
      cellArray->InsertCellPoint(i+1);
    }

    // Add connection between Y2 and Y1
    cellArray->InsertNextCell(3);
    cellArray->InsertCellPoint(0);
    cellArray->InsertCellPoint(2*numPointsEachSide);
    cellArray->InsertCellPoint(1);

    // Add the cap to the bottom
    cellArray->InsertNextCell(2*numPointsEachSide);
    for (int i = 1; i <= 2*numPointsEachSide; i++)
    {
      cellArray->InsertCellPoint(i);
    }
  }
  else
  {
    points->InsertPoint(0,0,0,this->SAD);
    points->InsertPoint(1, -2*this->Y2Jaw, -2*this->X2Jaw, -this->SAD );
    points->InsertPoint(2, -2*this->Y2Jaw, -2*this->X1Jaw, -this->SAD );
    points->InsertPoint(3, -2*this->Y1Jaw, -2*this->X1Jaw, -this->SAD );
    points->InsertPoint(4, -2*this->Y1Jaw, -2*this->X2Jaw, -this->SAD );

    cellArray->InsertNextCell(3);
    cellArray->InsertCellPoint(0);
    cellArray->InsertCellPoint(1);
    cellArray->InsertCellPoint(2);

    cellArray->InsertNextCell(3);
    cellArray->InsertCellPoint(0);
    cellArray->InsertCellPoint(2);
    cellArray->InsertCellPoint(3);

    cellArray->InsertNextCell(3);
    cellArray->InsertCellPoint(0);
    cellArray->InsertCellPoint(3);
    cellArray->InsertCellPoint(4);

    cellArray->InsertNextCell(3);
    cellArray->InsertCellPoint(0);
    cellArray->InsertCellPoint(4);
    cellArray->InsertCellPoint(1);

    // Add the cap to the bottom
    cellArray->InsertNextCell(4);
    cellArray->InsertCellPoint(1);
    cellArray->InsertCellPoint(2);
    cellArray->InsertCellPoint(3);
    cellArray->InsertCellPoint(4);
  }

  beamModelPolyData->SetPoints(points);
  beamModelPolyData->SetPolys(cellArray);
}
