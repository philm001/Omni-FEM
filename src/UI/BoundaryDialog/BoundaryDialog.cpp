#include <UI/BoundaryDialog/BoundaryDialog.h>

boundaryDialog::boundaryDialog(std::vector<magneticBoundary> boundaryList) : wxDialog(NULL, wxID_ANY, "Boundary Definition", wxDefaultPosition, wxSize(233, 148))
{
    wxFont *font = new wxFont(8.5, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    
    _magneticBoundaryList = boundaryList;
    
    for(std::vector<magneticBoundary>::iterator boundaryIterator = _magneticBoundaryList.begin(); boundaryIterator != _magneticBoundaryList.end(); ++boundaryIterator)
    {
        magneticBoundaryNameArray->Add(wxString(boundaryIterator->getBoundaryName()));
    }
    
    wxButton *addPropertyButton = new wxButton(this, propertiesDialogEnum::ID_ButtonAdd, "Add Property", wxPoint(12, 49), wxSize(125, 26));
    addPropertyButton->SetFont(*font);
    
    wxButton *deletePropertyButton = new wxButton(this, propertiesDialogEnum::ID_ButtonDelete, "Delete Property", wxPoint(12, 78), wxSize(125, 26));
    deletePropertyButton->SetFont(*font);
    
    wxButton *modifyPropertyButton = new wxButton(this, propertiesDialogEnum::ID_ButtonModify, "Modify Property", wxPoint(12, 107), wxSize(125, 26));
    modifyPropertyButton->SetFont(*font);
    
    wxButton *okButton = new wxButton(this, wxID_OK, "OK", wxPoint(146, 107), wxSize(75, 23));
    okButton->SetFont(*font);
    
    wxStaticText *name = new wxStaticText(this, wxID_ANY, "Name: ", wxPoint(12, 9), wxSize(38, 13));
    name->SetFont(*font);
    
    selection->Create(this, wxID_ANY, wxEmptyString, wxPoint(56, 6), wxSize(165, 21), *magneticBoundaryNameArray);
    selection->SetFont(*font);
    
    this->FitInside();
    this->SetMinSize(this->GetSize());
    this->SetMaxSize(this->GetSize());
}



void boundaryDialog::onAddProperty(wxCommandEvent &event)
{
    magneticBoundary magBC;
    _magBoundaryDialog->clearBoundary();
    if(_magBoundaryDialog->ShowModal() == wxID_OK)
    {
        _magBoundaryDialog->getBoundaryCondition(magBC);
        for(std::vector<magneticBoundary>::iterator boundaryIterator = _magneticBoundaryList.begin(); boundaryIterator != _magneticBoundaryList.end(); ++boundaryIterator)
        {
            if(boundaryIterator->getBoundaryName() == magBC.getBoundaryName())
            {
                wxMessageBox(magBC.getBoundaryName().append(" already exists. Choose a different name."), "Information", wxOK | wxICON_INFORMATION | wxCENTER);
                break;
            }
        }
        _magneticBoundaryList.push_back(magBC);
        selection->Append(magBC.getBoundaryName());
        selection->SetSelection(0);
    }
} 



void boundaryDialog::onDeleteProperty(wxCommandEvent &event)
{
    if(_magneticBoundaryList.size() > 0)
    {
        int currentSelection = selection->GetCurrentSelection();
        _magneticBoundaryList.erase(_magneticBoundaryList.begin() + currentSelection);
        selection->Delete(currentSelection);
        selection->SetSelection(0);
    }
}



void boundaryDialog::onModifyProperty(wxCommandEvent &event)
{
    magneticBoundary selectedBoundary;
    if(_magneticBoundaryList.size() > 0)
    {
        int currentSelection = selection->GetSelection();
        selectedBoundary = _magneticBoundaryList.at(currentSelection);
        _magBoundaryDialog->setBoundaryCondition(selectedBoundary);
        if(_magBoundaryDialog->ShowModal() == wxID_OK)
        {
            /*
             * This is a counter. The loop is checking to see if the user accidently changed the name of the material to one that is already there.
             * However, the one that the user wants to modify is still in the list. So, the program needs to skip
             * that one. Which, this counter will assit in that
             */ 
            int i = 0;
            _magBoundaryDialog->getBoundaryCondition(selectedBoundary);
            for(std::vector<magneticBoundary>::iterator boundaryIterator = _magneticBoundaryList.begin(); boundaryIterator != _magneticBoundaryList.end(); ++boundaryIterator)
            {
                if(boundaryIterator->getBoundaryName() == selectedBoundary.getBoundaryName() && (i != currentSelection))
                {
                    wxMessageBox(selectedBoundary.getBoundaryName().append(" already exists. Choose a different name."), "Information", wxOK | wxICON_INFORMATION | wxCENTER);
                    break;
                }
                
                i++;
            } 
            _magneticBoundaryList.at(currentSelection) = selectedBoundary;
            selection->SetString(currentSelection, selectedBoundary.getBoundaryName());
        }
        selection->SetSelection(0);
    }
}



std::vector<magneticBoundary> boundaryDialog::getBoundaryList()
{
    return _magneticBoundaryList;
}



boundaryDialog::~boundaryDialog()
{
    
}



wxBEGIN_EVENT_TABLE(boundaryDialog, wxDialog)
    EVT_BUTTON(propertiesDialogEnum::ID_ButtonAdd, boundaryDialog::onAddProperty)
    EVT_BUTTON(propertiesDialogEnum::ID_ButtonDelete, boundaryDialog::onDeleteProperty)
    EVT_BUTTON(propertiesDialogEnum::ID_ButtonModify, boundaryDialog::onModifyProperty)
wxEND_EVENT_TABLE()