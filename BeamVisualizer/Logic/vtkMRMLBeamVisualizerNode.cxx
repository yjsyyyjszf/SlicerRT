/*==============================================================================

  Program: 3D Slicer

  Copyright (c) Kitware Inc.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Csaba Pinter, PerkLab, Queen's University
  and was supported through the Applied Cancer Research Unit program of Cancer Care
  Ontario with funds provided by the Ontario Ministry of Health and Long-Term Care

==============================================================================*/

// MRMLBeamVisualizer includes
#include "vtkMRMLBeamVisualizerNode.h"

// MRML includes
#include <vtkMRMLScene.h>
#include <vtkMRMLAnnotationFiducialNode.h>
#include <vtkMRMLModelNode.h>

// VTK includes
#include <vtkObjectFactory.h>
#include <vtkSmartPointer.h>

// STD includes
#include <sstream>

//------------------------------------------------------------------------------
vtkMRMLNodeNewMacro(vtkMRMLBeamVisualizerNode);

//----------------------------------------------------------------------------
vtkMRMLBeamVisualizerNode::vtkMRMLBeamVisualizerNode()
{
  this->IsocenterFiducialNodeId = NULL;
  this->SourceFiducialNodeId = NULL;
  this->BeamModelNodeId = NULL;

  this->HideFromEditors = false;
}

//----------------------------------------------------------------------------
vtkMRMLBeamVisualizerNode::~vtkMRMLBeamVisualizerNode()
{
  this->SetIsocenterFiducialNodeId(NULL);
  this->SetSourceFiducialNodeId(NULL);
  this->SetBeamModelNodeId(NULL);
}

//----------------------------------------------------------------------------
void vtkMRMLBeamVisualizerNode::WriteXML(ostream& of, int nIndent)
{
  Superclass::WriteXML(of, nIndent);

  // Write all MRML node attributes into output stream
  vtkIndent indent(nIndent);

  {
    std::stringstream ss;
    if ( this->IsocenterFiducialNodeId )
      {
      ss << this->IsocenterFiducialNodeId;
      of << indent << " IsocenterFiducialNodeId=\"" << ss.str() << "\"";
     }
  }

  {
    std::stringstream ss;
    if ( this->SourceFiducialNodeId )
    {
      ss << this->SourceFiducialNodeId;
      of << indent << " SourceFiducialNodeId=\"" << ss.str() << "\"";
    }
  }

  {
    std::stringstream ss;
    if ( this->BeamModelNodeId )
      {
      ss << this->BeamModelNodeId;
      of << indent << " BeamModelNodeId=\"" << ss.str() << "\"";
     }
  }
}

//----------------------------------------------------------------------------
void vtkMRMLBeamVisualizerNode::ReadXMLAttributes(const char** atts)
{
  vtkMRMLNode::ReadXMLAttributes(atts);

  // Read all MRML node attributes from two arrays of names and values
  const char* attName;
  const char* attValue;
  while (*atts != NULL) 
    {
    attName = *(atts++);
    attValue = *(atts++);
    if (!strcmp(attName, "IsocenterFiducialNodeId")) 
      {
      std::stringstream ss;
      ss << attValue;
      this->SetAndObserveIsocenterFiducialNodeId(ss.str().c_str());
      }
    else if (!strcmp(attName, "SourceFiducialNodeId")) 
      {
      std::stringstream ss;
      ss << attValue;
      this->SetAndObserveSourceFiducialNodeId(ss.str().c_str());
      }
    else if (!strcmp(attName, "BeamModelNodeId")) 
      {
      std::stringstream ss;
      ss << attValue;
      this->SetAndObserveBeamModelNodeId(ss.str().c_str());
      }
    }
}

//----------------------------------------------------------------------------
// Copy the node's attributes to this object.
// Does NOT copy: ID, FilePrefix, Name, VolumeID
void vtkMRMLBeamVisualizerNode::Copy(vtkMRMLNode *anode)
{
  Superclass::Copy(anode);
  this->DisableModifiedEventOn();

  vtkMRMLBeamVisualizerNode *node = (vtkMRMLBeamVisualizerNode *) anode;

  this->SetAndObserveIsocenterFiducialNodeId(node->IsocenterFiducialNodeId);
  this->SetAndObserveSourceFiducialNodeId(node->SourceFiducialNodeId);
  this->SetAndObserveBeamModelNodeId(node->BeamModelNodeId);

  this->DisableModifiedEventOff();
  this->InvokePendingModifiedEvent();
}

//----------------------------------------------------------------------------
void vtkMRMLBeamVisualizerNode::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkMRMLNode::PrintSelf(os,indent);

  os << indent << "IsocenterFiducialNodeId:   " << this->IsocenterFiducialNodeId << "\n";
  os << indent << "SourceFiducialNodeId:   " << this->SourceFiducialNodeId << "\n";
  os << indent << "BeamModelNodeId:   " << this->BeamModelNodeId << "\n";
}

//----------------------------------------------------------------------------
void vtkMRMLBeamVisualizerNode::SetAndObserveIsocenterFiducialNodeId(const char* id)
{
  if (this->IsocenterFiducialNodeId)
  {
    this->Scene->RemoveReferencedNodeID(this->IsocenterFiducialNodeId, this);
  }

  this->SetIsocenterFiducialNodeId(id);

  if (id)
  {
    this->Scene->AddReferencedNodeID(this->IsocenterFiducialNodeId, this);
  }
}

//----------------------------------------------------------------------------
void vtkMRMLBeamVisualizerNode::SetAndObserveSourceFiducialNodeId(const char* id)
{
  if (this->SourceFiducialNodeId)
  {
    this->Scene->RemoveReferencedNodeID(this->SourceFiducialNodeId, this);
  }

  this->SetSourceFiducialNodeId(id);

  if (id)
  {
    this->Scene->AddReferencedNodeID(this->SourceFiducialNodeId, this);
  }
}

//----------------------------------------------------------------------------
void vtkMRMLBeamVisualizerNode::SetAndObserveBeamModelNodeId(const char* id)
{
  if (this->BeamModelNodeId)
  {
    this->Scene->RemoveReferencedNodeID(this->BeamModelNodeId, this);
  }

  this->SetBeamModelNodeId(id);

  if (id)
  {
    this->Scene->AddReferencedNodeID(this->BeamModelNodeId, this);
  }
}

//----------------------------------------------------------------------------
void vtkMRMLBeamVisualizerNode::UpdateReferenceID(const char *oldID, const char *newID)
{
  if (this->IsocenterFiducialNodeId && !strcmp(oldID, this->IsocenterFiducialNodeId))
    {
    this->SetAndObserveIsocenterFiducialNodeId(newID);
    }
  if (this->SourceFiducialNodeId && !strcmp(oldID, this->SourceFiducialNodeId))
    {
    this->SetAndObserveSourceFiducialNodeId(newID);
    }
  if (this->BeamModelNodeId && !strcmp(oldID, this->BeamModelNodeId))
    {
    this->SetAndObserveBeamModelNodeId(newID);
    }
}
