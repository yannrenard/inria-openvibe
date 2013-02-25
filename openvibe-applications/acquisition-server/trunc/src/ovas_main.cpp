#include "ovasCAcquisitionServerGUI.h"

#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#include <gtk/gtk.h>

#include <iostream>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace std;

int main(int argc, char ** argv)
{
//___________________________________________________________________//
//                                                                   //


	CKernelLoader l_oKernelLoader;

	cout<<"[  INF  ] Created kernel loader, trying to load kernel module"<<endl;
	CString m_sError;
#if defined OVAS_OS_Windows
	if(!l_oKernelLoader.load("../bin/OpenViBE-kernel-dynamic.dll", &m_sError))
#else
	if(!l_oKernelLoader.load("../lib/libOpenViBE-kernel-dynamic.so", &m_sError))
#endif
	{
			cout<<"[ FAILED ] Error loading kernel ("<<m_sError<<")"<<endl;
	}
	else
	{
		cout<<"[  INF  ] Kernel module loaded, trying to get kernel descriptor"<<endl;
		IKernelDesc* l_pKernelDesc=NULL;
		IKernelContext* l_pKernelContext=NULL;
		l_oKernelLoader.initialize();
		l_oKernelLoader.getKernelDesc(l_pKernelDesc);
		if(!l_pKernelDesc)
		{
			cout<<"[ FAILED ] No kernel descriptor"<<endl;
		}
		else
		{
			cout<<"[  INF  ] Got kernel descriptor, trying to create kernel"<<endl;
			l_pKernelContext=l_pKernelDesc->createKernel("acquisition-server", "../share/openvibe.conf");
			if(!l_pKernelContext)
			{
				cout<<"[ FAILED ] No kernel created by kernel descriptor"<<endl;
			}
			else
			{
				OpenViBEToolkit::initialize(*l_pKernelContext);

// For Mister Vincent !
#ifdef OVAS_OS_Windows
#ifndef NDEBUG
				//_asm int 3;
#endif
#endif

				IConfigurationManager& l_rConfigurationManager=l_pKernelContext->getConfigurationManager();

				l_pKernelContext->getPluginManager().addPluginsFromFiles(l_rConfigurationManager.expand("${Kernel_Plugins}"));

				//initialise Gtk before 3D context
				// g_thread_init has been deprecated since version 2.32 and should not be used in newly-written code. This function is no longer necessary
				// g_thread_init(NULL);
				gdk_threads_init();
				gtk_init(&argc, &argv);

				// gtk_rc_parse("../share/openvibe-applications/designer/interface.gtkrc");

#if 0 // This is not needed in the acquisition server
				if(l_rConfigurationManager.expandAsBoolean("${Kernel_3DVisualisationEnabled}"))
				{
					l_pKernelContext->getVisualisationManager().initialize3DContext();
				}
#endif

				{
					// If this is encapsulated by gdk_threads_enter() and gdk_threads_exit(), m_pThread->join() can hang when gtk_main() returns before destructor of app has been called.
					OpenViBEAcquisitionServer::CAcquisitionServerGUI app(*l_pKernelContext);

					try
					{
						gdk_threads_enter();	
						gtk_main();
						gdk_threads_leave();			
					}
					catch(...)
					{
						l_pKernelContext->getLogManager() << LogLevel_Fatal << "Catched top level exception\n";
					}
				}

				cout<<"[  INF  ] Application terminated, releasing allocated objects"<<endl;

				OpenViBEToolkit::uninitialize(*l_pKernelContext);

				l_pKernelDesc->releaseKernel(l_pKernelContext);
			}
		}
		l_oKernelLoader.uninitialize();
		l_oKernelLoader.unload();
	}

	return 0;
}
