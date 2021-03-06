/*
 * PostProcessorView.h
 *
 *  Created on: Nov 22, 2018
 *      Author: phillip
 */

#ifndef INCLUDE_POST_PROCESSOR_POSTPROCESSORVIEW_H_
#define INCLUDE_POST_PROCESSOR_POSTPROCESSORVIEW_H_

#include "PostProcessorWindow.h"

#include "QMainWindow"
#include "QObject"
#include "QString"
#include "QAction"
#include "QList"
#include "QToolBar"
#include "QStringList"
#include "QList"

#include <wx/stdpaths.h>
#include <wx/string.h>

#include "vtkPVGeneralSettings.h"

#include "pqSGExportStateWizard.h"

#include "pqAlwaysConnectedBehavior.h"
#include "pqApplyBehavior.h"
#include "pqDefaultViewBehavior.h"

#include "pqApplicationCore.h"
#include "pqAutoLoadPluginXMLBehavior.h"
#include "pqInterfaceTracker.h"
#include "pqParaViewBehaviors.h"
#include "pqParaViewMenuBuilders.h"
#include "pqStandardViewFrameActionsImplementation.h"

#include "pqAxesToolbar.h"
#include "pqLoadDataReaction.h"
#include "pqMainControlsToolbar.h"
#include "pqRepresentationToolbar.h"
#include "pqSetName.h"

#include "pqParaViewBehaviors.h"
#include "pqLoadDataReaction.h"
#include "pqPipelineBrowserWidget.h"
#include "pqPipelineSource.h"

#include "pqOutputPort.h"
#include "pqView.h"
#include "pqActiveObjects.h"

#include "vtkSMProxyManager.h"
#include "vtkSMReaderFactory.h"


/*
 * This class handles the functionality of the window
 */
class postProcessorView : public QMainWindow
{
//	Q_OBJECT
	typedef QMainWindow Superclass;

private:
	Q_DISABLE_COPY(postProcessorView)
	class p_pqInternal;
	p_pqInternal *p_Internals;

public:
	postProcessorView();

	void loadData();

	virtual ~postProcessorView() {};

protected slots:
	void showHelpProxy(const QString &groupName, const QString &proxyName);
};



#endif /* INCLUDE_POST_PROCESSOR_POSTPROCESSORVIEW_H_ */
