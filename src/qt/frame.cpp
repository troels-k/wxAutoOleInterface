/////////////////////////////////////////////////////////////////////////////
// Name:        src/qt/frame.cpp
// Author:      Peter Most
// Id:          $Id$
// Copyright:   (c) Peter Most
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/frame.h"
#include "wx/menu.h"
#include "wx/qt/utils.h"
#include "wx/qt/converter.h"

wxFrame::wxFrame()
{
}

wxFrame::~wxFrame()
{
    SendDestroyEvent();
    
    delete m_qtFrame;
}


wxFrame::wxFrame( wxWindow *parent, wxWindowID id, const wxString& title,
        const wxPoint& pos, const wxSize& size, long style, const wxString& name )
{
    Create( parent, id, title, pos, size, style, name );
}

bool wxFrame::Create( wxWindow *parent, wxWindowID id, const wxString& title,
    const wxPoint& pos, const wxSize& size, long style, const wxString& name )
{
    // It might not be necessary to check whether the underlying Qt object has been
    // create, but just in case we do it anyway:

    if ( GetHandle() == NULL )
        m_qtFrame = wxQtCreateWidget< wxQtFrame >( this, parent );

    return wxFrameBase::Create( parent, id, title, pos, size, style, name );
}

void wxFrame::SetMenuBar( wxMenuBar *menuBar )
{
    m_qtFrame->setMenuBar( menuBar->GetHandle() );

    wxFrameBase::SetMenuBar( menuBar );
}

void wxFrame::SetStatusBar( wxStatusBar *statusBar )
{
    m_qtFrame->setStatusBar( statusBar->GetHandle() );

    // Update statusbar sizes now that it has a size
    statusBar->Refresh();

    wxFrameBase::SetStatusBar( statusBar );
}

QMainWindow *wxFrame::GetHandle() const
{
    return m_qtFrame;
}

QWidget *wxFrame::QtGetScrollBarsContainer() const
{
    return m_qtFrame->centralWidget();
}

void wxFrame::SetWindowStyleFlag( long style )
{
    wxWindow::SetWindowStyleFlag( style );
    
    Qt::WindowFlags qtFlags = GetHandle()->windowFlags();
    
    wxCHECK_RET( !HasFlag( wxFRAME_FLOAT_ON_PARENT ) && !HasFlag( wxTINY_CAPTION ) , 
                 "wxFRAME_FLOAT_ON_PARENT, wxTINY_CAPTION not supported. Use wxFRAME_TOOL_WINDOW instead." );
    wxCHECK_RET( !HasFlag( wxFRAME_TOOL_WINDOW ) || HasFlag( wxFRAME_NO_TASKBAR ) , 
                 "wxFRAME_TOOL_WINDOW without wxFRAME_NO_TASKBAR not supported." );
    
    if ( HasFlag( wxFRAME_TOOL_WINDOW ) )
    {
        qtFlags &= ~Qt::WindowType_Mask;
        qtFlags = Qt::Tool;
    }
    else if ( HasFlag( wxFRAME_NO_TASKBAR ) )
    {
        qtFlags &= ~Qt::WindowType_Mask;
        qtFlags = Qt::Dialog;
    }
    
    GetHandle()->setWindowFlags( qtFlags );
}
//=============================================================================

wxQtFrame::wxQtFrame( wxFrame *frame, QWidget *parent )
    : wxQtEventForwarder< wxFrame, QMainWindow >( frame, parent )
{
    setCentralWidget(new QWidget());
}
