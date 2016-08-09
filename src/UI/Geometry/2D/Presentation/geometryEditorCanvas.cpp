#include <UI/openGLGeometry.h>
#include <string.h>

#include <math.h>

class tempXCoor;
class tempXCoor;
class tempXCoor;
using namespace std;

geometryEditorCanvas::geometryEditorCanvas(wxWindow *par, const wxPoint &position, const wxSize &size) : wxGLCanvas(par, wxID_ANY, NULL, position, size, wxBORDER_DOUBLE | wxBORDER_RAISED)
{
 //   debugCoordinate = new wxStaticText(this, wxID_ANY, "None", wxPoint(0, 10));
 //   debugPixelCoordinate = new wxStaticText(this, wxID_ANY, "None");
    // At startup, the canvas size is initially 613x241 pixels
    debugCoordinate = new wxGLString("None");
    debugPixelCoordinate = new wxGLString("none");
    
    this->SetLabel("none");
    
	geometryContext = new wxGLContext(this);
	wxGLCanvas::SetCurrent(*geometryContext);
	canvasHeight = (double)this->GetSize().y;
	canvasWidth = (double)this->GetSize().x;
	
	coordinateFactorWidth = canvasWidth / (double)factor;
	coordinateFactorHeight = canvasHeight / (double)factor;
	
	glViewport(0, 0, canvasWidth, canvasHeight);// Set the viewport to see the rendering
	
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);// This will control the color of the background for the drawing canvas

	glMatrixMode(GL_MODELVIEW);// The matrix mode specifies which matrix stack is the target for matrix operations
	glLoadIdentity();// Initial value
//	glTranslated((float)this->GetSize().x / 2.0f, (float)this->GetSize().y / 2.0f, 0.0f);// This will move the camera to the center of the screen
    
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/*	
		This will setup the screen such that the x value will extend from the left (x = 0) to the right (x = canvas->GetSize().x)
		As for the y value, it will extend from the top (y = 0) to the bottom (y = canvas->GetSize().y)
		By default, the screen is the the unit square. This command changes that 
	*/
	glOrtho(0.0, canvasWidth, canvasHeight, 0.0, 1.0, -1.0);
	
	/* Ensure that there are no errors */
	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
	{
	//	wxMessageBox("Error - " + gluErrorString(error));
		return;
	}

	boundaryList.Add("<None>");
}


void geometryEditorCanvas::render()
{

	
    
    
}


void geometryEditorCanvas::drawGrid()
{
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
    glPushMatrix();
	
//	glTranslatef(canvasWidth / 2.0f, canvasHeight / 2.0f, 0.0f);
	
    glLineWidth(1.0);
    
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x1C47);
    
    glBegin(GL_LINES);
    
	for(int heightLoop = (-10 + cameraY * Ycoeff); heightLoop <= (10 + cameraY * Ycoeff); heightLoop++)
	{
		
		for(int widthLoop = (-10 + cameraX * Xcoeff); widthLoop <= (10 + cameraX * Xcoeff); widthLoop++)
		{
			int tempYPixel = convertToYPixel((double)heightLoop);
			int tempXPixel = convertToXPixel((double)widthLoop);
			glBegin(GL_POINTS);
				glColor3f( 0.f, 0.f, 0.f );
				glVertex2i(tempXPixel, tempYPixel);
			glEnd();
		}
	}
    
    
 /* Listed below is the code from Github for Agros2D. The code is for drawing a grid. This needs to be integrated into Omni-FEM */

/* SceneView::paintGrid()

    loadProjection2d(true);

    Point cornerMin = position(Point(0, 0));
    Point cornerMax = position(Point(contextWidth(), contextHeight()));

    glDisable(GL_DEPTH_TEST);

    // heavy line
    int heavyLine = 5;

    glLineWidth(1.0);
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x1C47);
    glBegin(GL_LINES);

    if ((((cornerMax.x-cornerMin.x)/Util::config()->gridStep + (cornerMin.y-cornerMax.y)/Util::config()->gridStep) < 200) &&
            ((cornerMax.x-cornerMin.x)/Util::config()->gridStep > 0) && ((cornerMin.y-cornerMax.y)/Util::config()->gridStep > 0))
    {
     * The gridStep is set to 0.05
        // vertical lines
        for (int i = cornerMin.x/Util::config()->gridStep - 1; i < cornerMax.x/Util::config()->gridStep + 1; i++)
        {
            if (i % heavyLine == 0)
                glColor3d(Util::config()->colorCross.redF(),
                          Util::config()->colorCross.greenF(),
                          Util::config()->colorCross.blueF());
            else
                glColor3d(Util::config()->colorGrid.redF(),
                          Util::config()->colorGrid.greenF(),
                          Util::config()->colorGrid.blueF());
            glVertex2d(i*Util::config()->gridStep, cornerMin.y);
            glVertex2d(i*Util::config()->gridStep, cornerMax.y);
        }

        // horizontal lines
        for (int i = cornerMax.y/Util::config()->gridStep - 1; i < cornerMin.y/Util::config()->gridStep + 1; i++)
        {
            if (i % heavyLine == 0)
                glColor3d(Util::config()->colorCross.redF(),
                          Util::config()->colorCross.greenF(),
                          Util::config()->colorCross.blueF());
            else
                glColor3d(Util::config()->colorGrid.redF(),
                          Util::config()->colorGrid.greenF(),
                          Util::config()->colorGrid.blueF());
            glVertex2d(cornerMin.x, i*Util::config()->gridStep);
            glVertex2d(cornerMax.x, i*Util::config()->gridStep);
        }        
    }
    glEnd();
    glDisable(GL_LINE_STIPPLE);

    if (m_scene->problemInfo()->problemType == ProblemType_Axisymmetric)
    {
        drawBlend(cornerMin,
                  Point(0, cornerMax.y),
                  Util::config()->colorGrid.redF(),
                  Util::config()->colorGrid.greenF(),
                  Util::config()->colorGrid.blueF(), 0.25);
    }

    // axes
    glColor3d(Util::config()->colorCross.redF(),
              Util::config()->colorCross.greenF(),
              Util::config()->colorCross.blueF());
    glLineWidth(1.5);
    glBegin(GL_LINES);
    // y axis
    glVertex2d(0, cornerMin.y);
    glVertex2d(0, cornerMax.y);
    // x axis
    glVertex2d(((m_scene->problemInfo()->problemType == ProblemType_Axisymmetric) ? 0 : cornerMin.x), 0);
    glVertex2d(cornerMax.x, 0);
    glEnd();

 */
    
}



void geometryEditorCanvas::onKeyDown(wxKeyEvent &event)
{
	std::vector<int> deletednodes;
	
	addNode(5, 5, 0);
	
	
	if(event.GetKeyCode() != DEL_KEY)
	{
		if(event.GetKeyCode() == LETTER_W || event.GetKeyCode() == LETTER_w)
		{
			cameraY -= 10.0f;
		}
		else if(event.GetKeyCode() == LETTER_S || event.GetKeyCode() == LETTER_s)
		{
			cameraY += 10.0f;
		}	
		else if(event.GetKeyCode() == LETTER_A || event.GetKeyCode() == LETTER_a)
		{
			cameraX -= 10.0f;
		}
		else if(event.GetKeyCode() == LETTER_d || event.GetKeyCode() == LETTER_D)
		{	
			cameraX += 10.0f;
		}
    
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		glLoadIdentity();
		
		glTranslatef(cameraX, cameraY, 0.0f);
		glPushMatrix();
	
	}
	else
	{
		if(nodeList.size() == 1)
		{
			nodeList.clear();
		}
		else
		{
			for(std::vector<node>::iterator nodeIterator = nodeList.begin(); nodeIterator != nodeList.end(); ++nodeIterator)
			{
				if(nodeIterator->getIsSelected())
				{
					nodeList.erase(nodeIterator);
					nodeIterator = nodeList.begin();
					if(firstSelectedNodeIndex != -1 && (nodeIterator->getCenterXPixel() == nodeList[firstSelectedNodeIndex].getCenterXPixel()) && (nodeIterator->getCenterYPixel() == nodeList[firstSelectedNodeIndex].getCenterYPixel()))
					{
						firstSelectedNodeIndex = -1;
					}
				}
			}
			
			if(nodeList.size() == 1 && nodeList[0].getIsSelected() == true)
			{
				nodeList.clear();
			}
				
		}
	}

    this->Refresh();// This will force the canvas to experience a redraw event
}



void geometryEditorCanvas::onMouseMove(wxMouseEvent &event)
{
	mouseX = event.GetX();
	mouseY = event.GetY();
	std::stringstream stringMouseXCoor, stringMouseYCoor, stringMousePixelX, stringMousePixelY;
	
	// Converts the mouse pointer into a cartesian graph position
	mouseGraphX = convertToXCoordinate((double)mouseX, cameraX);
	mouseGraphY = convertToYCoordinate((double)mouseY, cameraY);
    
    stringMouseXCoor << std::fixed << setprecision(3) << mouseGraphX;
	stringMouseYCoor << std::fixed << setprecision(3) << mouseGraphY;
    
    stringMousePixelX << std::fixed << setprecision(1) << mouseX;
    stringMousePixelY << std::fixed << setprecision(1) << mouseY;
    
 //   debugPixelCoordinate->SetLabel(stringMousePixelX.str() + " " + stringMousePixelY.str());
 //   debugCoordinate->SetLabel(stringMouseXCoor.str() + " " + stringMouseYCoor.str());
  //  this->Refresh();

}



double geometryEditorCanvas::convertToXCoordinate(int xPixel)
{
	return (Xcoeff * ((double)xPixel) - graphOffset);
}


double geometryEditorCanvas::convertToXCoordinate(int xPixel, int cameraOffset)
{
	return (Xcoeff * ((double)xPixel - cameraOffset) - graphOffset);
}


double geometryEditorCanvas::convertToYCoordinate(int yPixel)
{
	return (Ycoeff* ((double)yPixel) + graphOffset);
}


double geometryEditorCanvas::convertToYCoordinate(int yPixel, int cameraOffset)
{
	return (Ycoeff* ((double)yPixel - cameraOffset) + graphOffset);
}


int geometryEditorCanvas::convertToXPixel(double XCoor)
{
	return (int)((XCoor + graphOffset) / Xcoeff);
}


int geometryEditorCanvas::convertToXPixel(double XCoor, int cameraOffset)
{
	return (int)((XCoor + graphOffset) / Xcoeff) + cameraOffset;
}


int geometryEditorCanvas::convertToYPixel(double YCoor)
{
	return (int)((YCoor - graphOffset) / Ycoeff) + 3; // Due to there being errors in the double data type, a small offset of 3 needed to be introduced
}

int geometryEditorCanvas::convertToYPixel(double YCoor, int cameraOffset)
{
	return (int)((YCoor - graphOffset) / Ycoeff) + 3 + cameraOffset; // Due to there being errors in the double data type, a small offset of 3 needed to be introduced
}


void geometryEditorCanvas::convertPixelToCoor(double xPoint, double yPoint, int &xPixel, int &yPixel)
{
	xPixel = convertToXPixel(xPoint);
	
	yPixel = convertToYPixel(yPoint);
}


void geometryEditorCanvas::onGeometryPaint(wxPaintEvent &event)
{
	wxGLCanvas::SetCurrent(*geometryContext);// This will make sure the the openGL commands are routed to the wxGLCanvas object
	wxPaintDC dc(this);// This is required for drawing
	
		//Reset to modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
    glPushMatrix();
	
	glTranslatef(canvasWidth / 2.0f, canvasHeight / 2.0f, 0.0f);
	
    glClear(GL_COLOR_BUFFER_BIT);
	drawGrid();
	int nodeListSize = nodeList.size();
    
	
	
	if(lineList.size() > 0)
	{
		for(int i = 0; i < lineList.size(); i++)
		{
			lineList[i].draw(nodeList[lineList[i].getFirstNodeIndex()].getCenterXPixel(), nodeList[lineList[i].getFirstNodeIndex()].getCenterYPixel(), nodeList[lineList[i].getSecondNodeIndex()].getCenterXPixel(), nodeList[lineList[i].getSecondNodeIndex()].getCenterYPixel());
		}
	}
	
	if(nodeList.size() > 0)
	{
		for(std::vector<node>::iterator nodeIterator = nodeList.begin(); nodeIterator != nodeList.end(); ++nodeIterator)
		{
			int tempX = 0;
			int tempY = 0;
			convertPixelToCoor(nodeIterator->getCenterXCoordinate(), nodeIterator->getCenterYCoordinate(), tempX, tempY);

			nodeIterator->setCenterXPixel(tempX);
			nodeIterator->setCenterYPixel(tempY);
		
			nodeIterator->draw();
		}
	}
	
	SwapBuffers();// Display the output
}


void geometryEditorCanvas::onResize(wxSizeEvent &event)
{
	canvasHeight = (double)this->GetSize().y;
	canvasWidth = (double)this->GetSize().x;
	
	this->Refresh();
}


void geometryEditorCanvas::toggleBlockListCreation()
{
	if(blockLabelCreationIsEnabled == false)
		blockLabelCreationIsEnabled = true;
	else
		blockLabelCreationIsEnabled = false;
}

void geometryEditorCanvas::onMouseWheel(wxMouseEvent &event)
{
	glMatrixMode(GL_MODELVIEW);
//	glPopMatrix();
//	glLoadIdentity();
	
	if(event.GetWheelRotation() > 0)
	{
		glOrtho(0.0, canvasWidth * zoomFactor, canvasHeight * zoomFactor, 0.0, 1.0, -1.0);
		//glScaled(zoomFactor, zoomFactor, 0.0d);
		totalZoom *= zoomFactor;
	}
	else if(event.GetWheelRotation() < 0)
	{
		glOrtho(0.0, (double)canvasWidth / zoomFactor, (double)canvasHeight / zoomFactor, 0.0, 1.0, -1.0);
		//glScaled(1.0d / zoomFactor, 1.0d / zoomFactor, 0.0d);
		totalZoom /= zoomFactor;
	}
	
//	glPushMatrix();
	
    this->Refresh();// This will force the canvas to experience a redraw event
}


void geometryEditorCanvas::onEnterWindow(wxMouseEvent &event)
{
	this->SetFocus();
}



void geometryEditorCanvas::onLeavingWindow(wxMouseEvent &event)
{
	this->GetParent()->SetFocus();
}



void geometryEditorCanvas::onMouseLeftDown(wxMouseEvent &event)
{
	wxGLCanvas::SetCurrent(*geometryContext);
	bool nodeSelected = false;
	
	
	if(blockLabelCreationIsEnabled)
	{
		if(!(nodeList.size() >= 2 && (lineList.size() > 0 || arcList.size() > 0)))
			return;
	}
	else if(nodeList.size() == 0)
	{
		addNode(mouseGraphX, mouseGraphY, 0);
		this->Refresh();
		return;
	}
	
	for(int i = 0; i < lineList.size(); i++)
	{
		
	}
	
    /* This section will be for node handling */
	for(int i = 0; i < nodeList.size(); i++)
	{
		if((((mouseX - cameraX) >= nodeList[i].getCenterXPixel() - 5) && ((mouseX - cameraX) <= nodeList[i].getCenterXPixel() + 5) && ((mouseY - cameraY) <= nodeList[i].getCenterYPixel() + 5) && ((mouseY - cameraY) >=nodeList[i].getCenterYPixel() - 5)) && nodeSelected == false)
		{
			if(blockLabelCreationIsEnabled)
				return;
			else if(event.ShiftDown())
			{
				/*	This section will first check to see if any other shape is selected.
					If the shape is selected, then toggle the selection.
					As a last step, the function will then toggle select the selected node
				*/
				if(blockLabelList.size() > 0)
				{
					for(int j = 0; j < blockLabelList.size(); j++)
					{
						if(blockLabelList[j].getIsSelected())
							blockLabelList[j].toggleSelect();
					}
				}
				
				if(arcList.size() > 0)
				{
					for(int j = 0; j < arcList.size(); j++)
					{
						if(arcList[j].getIsSelected())
							arcList[j].toggleSelect();
					}
				}
				
				if(lineList.size() > 0)
				{
					for(int j = 0; j < lineList.size(); j++)
					{
						if(lineList[j].getIsSelected())
							lineList[j].toggleSelect();
					}
				}
				
				nodeList[i].toggleSelect();
				this->Refresh();
				return;
			}
			else if(firstSelectedNodeIndex == -1)
			{
				for (int j = 0; j < nodeList.size(); j++)
				{
					if(nodeList[j].getIsSelected())
						nodeList[j].toggleSelect();
				}
				nodeList[i].toggleSelect();
				firstSelectedNodeIndex = i;
				this->Refresh();
				return;
			}
			else
			{
				if(lineCreationFlag)
				{
					
					double tempXCoor, tempYCoor;
					for(int k = 0; k < lineList.size(); k++)
					{
						if(getIntersection(firstSelectedNodeIndex, i, k, tempXCoor, tempYCoor) == true)
						{
							addNode(tempXCoor, tempYCoor, 0.01);
						}
					}
					
					addLineSegment(firstSelectedNodeIndex, nodeList[i].getNodeIndex(), NULL);
				}
				else
				{
					/* 
						This section of the code will calculate many features that make up the arc such as the arc Legnth.
						The add arcSegment will calculate anything needed for intercestions
					*/
					arcShape arcSegment;
					
					/* First, we will create a dialog box so that some basic propeties of the arc can be set by the user */
					wxDialog *arcSegmentDlg = new wxDialog(NULL, wxID_ANY, "Arc Segment Dialog", wxDefaultPosition, wxSize(269, 205));
					wxPanel *dialogPanel = new wxPanel(arcSegmentDlg);
					wxButton *buttonOk = new wxButton(dialogPanel, wxID_OK, "Ok", wxPoint(101, 165), wxSize(75, 23));
					wxButton *buttonCancel = new wxButton(dialogPanel, wxID_CANCEL, "Cancel", wxPoint(182, 165), wxSize(75, 23));
					
					wxFont *font = new wxFont(8.5, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
					
					wxStaticText *arcAngleText = new wxStaticText(dialogPanel, wxID_ANY, "Arc Angle (deg):", wxPoint(12, 38), wxSize(80,13));
					arcAngleText->SetFont(*font);
					
					wxStaticText *maxSegmentText = new wxStaticText(dialogPanel, wxID_ANY, "Max Segment:", wxPoint(12, 83), wxSize(72,13));
					maxSegmentText->SetFont(*font);
					
					wxStaticText *boundaryText = new wxStaticText(dialogPanel, wxID_ANY, "Boundary", wxPoint(12, 125), wxSize(52,13));
					boundaryText->SetFont(*font);
					
					wxTextCtrl *arcAngle = new wxTextCtrl(dialogPanel, wxID_ANY, "180", wxPoint(101, 35), wxSize(156, 20));
					wxTextCtrl *maxSegment = new wxTextCtrl(dialogPanel, wxID_ANY, "3", wxPoint(101, 80), wxSize(156, 20));

					wxComboBox *boundaryComboBox = new wxComboBox(dialogPanel, wxID_ANY, "", wxPoint(101, 122), wxSize(156, 21), boundaryList);
					
					if(arcSegmentDlg->ShowModal() == wxID_OK)
					{
						/* If the user clicked on ok to idicate they are done, we will then set the properties to an arc segment and add the segment to the geometry */
						double arcAngleVal = 0.0;
						double maxSegmentVal = 0.0;
						int index = boundaryComboBox->GetSelection();
						wxString test = boundaryList.Item(index);
						
						arcAngle->GetValue().ToDouble(&arcAngleVal);
						arcSegment.setArcLength(arcAngleVal);// This needs to be changed to reflect that it is really the arc angle and not length
						
						maxSegment->GetValue().ToDouble(&maxSegmentVal);
						arcSegment.setMaxSideLength(maxSegmentVal);
						
						//arcSegment.setBoundaryMarker(test);
						arcSegment.setBoundaryMarker(test);
						
						arcSegment.setFirstNodeIndex(firstSelectedNodeIndex);
						arcSegment.setSecondNodeIndex(nodeList[i].getNodeIndex());
						
						addArcSegment(arcSegment);
					}
				}
				
				nodeList[firstSelectedNodeIndex].toggleSelect();
				firstSelectedNodeIndex = -1;
				this->Refresh();
				return;
			}
		}			
	}
	
	addNode(mouseGraphX, mouseGraphY, 0);
	this->Refresh();
}

wxBEGIN_EVENT_TABLE(geometryEditorCanvas, wxGLCanvas)
	EVT_PAINT(geometryEditorCanvas::onGeometryPaint)
	EVT_SIZE(geometryEditorCanvas::onResize)
    EVT_KEY_DOWN(geometryEditorCanvas::onKeyDown)
	EVT_MOUSEWHEEL(geometryEditorCanvas::onMouseWheel)
	EVT_ENTER_WINDOW(geometryEditorCanvas::onEnterWindow)
	EVT_LEAVE_WINDOW(geometryEditorCanvas::onLeavingWindow)
	EVT_MOTION(geometryEditorCanvas::onMouseMove)
	EVT_LEFT_DOWN(geometryEditorCanvas::onMouseLeftDown)
wxEND_EVENT_TABLE()

//nodePoint::drawnodePoint(GLdouble xcoordinate, GL)