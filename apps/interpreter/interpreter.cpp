// Includes, namespace and prototypes

#include "interpreter.h"
using namespace AGK;
app App;

void CallbackOnAppDebugInfo( const char *szType, const char *szData ) { App.OnAppDebugInfo( szType, szData ); }
void CallbackOnAppWarning( const char *szMsg ) { App.OnAppWarning( szMsg ); }
void CallbackOnAppError( const char *szMsg ) { App.OnAppError( szMsg ); }

void GeneralError( const char *msg )
{
	try { agk::Error( msg ); } catch(...) {}
	agk::Message( msg );
	//App.AppQuit();
}

app::app()
{
	m_iBroadcastPort = 5689;
	m_iConnectionPort = 5690;
	m_pConnection = 0;

	m_iAppControlStage = APP_IDLE;
	m_iPingResponded = 1;
	m_fPingStart = 0;
}

app::~app()
{
	if ( m_FileReceiver.IsRunning() ) 
	{
		m_FileReceiver.Stop();
	}

	if ( m_pConnection ) m_pConnection->Close();

	m_FileReceiver.Join();

	if ( m_pConnection ) delete m_pConnection;
	m_pConnection = 0;
}

float timeTaken = 0;

void app::UpdateInterpreterAspect ( void )
{
	g_iPrevWidth = agk::GetDeviceWidth();
	g_iPrevHeight = agk::GetDeviceHeight();

	//static float fAspect = agk::GetDisplayAspect();
	agk::SetVirtualResolution ( 100, 100 );
	float fAspect = agk::GetDeviceWidth() / float(agk::GetDeviceHeight());
	agk::SetDisplayAspect(fAspect);
	if ( fAspect >= 1.0f )
	{
		// landscape or square
		if ( g_pAGKBackdrop.pSprite ) 
		{
			g_pAGKBackdrop.pSprite->SetSize ( 100, 100*fAspect );
			g_pAGKBackdrop.pSprite->SetPosition ( 0, (100-(100*fAspect))/2.0f );
		}
		
		if ( g_pAGKBackdropSpinner.pSprite ) 
		{
			g_pAGKBackdropSpinner.pSprite->SetSize ( 62/fAspect, 62 );
			g_pAGKBackdropSpinner.pSprite->SetPositionByOffset ( 50, 48 );
		}

		if ( g_pAGKBackdropLogo.pSprite ) 
		{
			g_pAGKBackdropLogo.pSprite->SetSize ( 62/fAspect, 62 );
			g_pAGKBackdropLogo.pSprite->SetPositionByOffset ( 50, 48 );
		}
	}
	else
	{
		// portrait
		if ( g_pAGKBackdrop.pSprite ) 
		{
			g_pAGKBackdrop.pSprite->SetSize ( 100/fAspect, 100 );
			g_pAGKBackdrop.pSprite->SetPosition ( (100-(100/fAspect))/2.0f, 0 );
		}

		if ( g_pAGKBackdropSpinner.pSprite ) 
		{
			g_pAGKBackdropSpinner.pSprite->SetSize ( 65, 65*fAspect );
			g_pAGKBackdropSpinner.pSprite->SetPositionByOffset ( 50, 48 );
		}

		if ( g_pAGKBackdropLogo.pSprite ) 
		{
			g_pAGKBackdropLogo.pSprite->SetSize ( 65, 65*fAspect );
			g_pAGKBackdropLogo.pSprite->SetPositionByOffset ( 50, 48 );
		}
	}

	if ( g_pAGKBackdropPower.pSprite ) 
	{
		g_pAGKBackdropPower.pSprite->SetSize ( 10/fAspect, 10 );
		g_pAGKBackdropPower.pSprite->SetPosition ( 99-(10/fAspect), 1 );
	}
}

// Begin app, called once at the start
void app::Begin( void )
{
	agk::SetCurrentDir ( "" );
	agk::MakeFolder ( "media" );
	agk::SetCurrentDir ( "media" );

	agk::SetDisplayAspect( (float)g_dwDeviceWidth/(float)g_dwDeviceHeight );

	if ( agk::GetFileExists( "bytecode.byc" ) == 1 )
	{
		agk::SetWindowAllowResize(0);

		m_iStandAlone = 1;
		m_iDebugMode = 0;
		m_iStepMode = 0;
		m_iAppControlStage = APP_RUNNING;

		// load stand alone app
		if ( !m_sProgram.LoadBytecode( "bytecode.byc" ) )
		{
			AppQuit();
		}
	}
	else
	{
		agk::FirebaseSetup();
		agk::SetWindowAllowResize(1);
        agk::SetOrientationAllowed(1, 1, 1, 1);
		m_iStandAlone = 0;
		m_iDebugMode = 0;
		m_iStepMode = 0;
		m_iAppControlStage = APP_NETWORK_SETUP;
		m_sIP.SetStrUTF8( "<None>" );

		//g_pArialImage = new cImage( "/Avenir.png" );
		//g_pAsciiImage = new cImage( "ascii.png" );

		g_pAGKBackdrop.pImage = new cImage ( "/interpreter-backdrop.png" );
		g_pAGKBackdrop.pSprite = new cSprite ( g_pAGKBackdrop.pImage );
        g_pAGKBackdrop.pSprite->FixToScreen ( 1 );
		g_pAGKBackdrop.pSprite->SetTransparency( 0 );
		g_pAGKBackdrop.pSprite->SetDepth( 100 );
		g_pAGKBackdropSpinner.pImage = new cImage ( "/interpreter-spinner.png" );
		g_pAGKBackdropSpinner.pSprite = new cSprite ( g_pAGKBackdropSpinner.pImage );
		g_pAGKBackdropSpinner.pSprite->SetAlpha ( 16 );
		g_pAGKBackdropSpinner.pSprite->FixToScreen ( 1 );
		g_pAGKBackdropSpinner.pSprite->SetDepth( 80 );
		g_pAGKBackdropLogo.pImage = new cImage ( "/interpreter-logo.png" );
		g_pAGKBackdropLogo.pSprite = new cSprite ( g_pAGKBackdropLogo.pImage );
		g_pAGKBackdropLogo.pSprite->FixToScreen ( 1 );
		g_pAGKBackdropLogo.pSprite->SetDepth ( 90 );
		g_pAGKBackdropPower.pImage = new cImage ( "/interpreter-power.png" );
		g_pAGKBackdropPower.pSprite = new cSprite ( g_pAGKBackdropPower.pImage );
		g_pAGKBackdropPower.pSprite->FixToScreen ( 1 );
		g_pAGKBackdropPower.pSprite->SetDepth ( 70 );
		for ( DWORD n=0; n<5; n++ )
		{
			g_pText[n] = new AGK::cText ();
			g_pText[n]->SetAlignment ( 1 );
			//g_pText[n]->SetFontImage( g_pArialImage );
			g_pText[n]->SetFont( 0 );
			g_pText[n]->FixToScreen ( 1 );
		}
		g_pText[0]->SetPosition ( 50, 80.0f );
		g_pText[0]->SetColor ( 0,0,0,192 );
		g_pText[0]->SetSize ( 5.0f );
		g_pText[1]->SetPosition ( 50, 86.0f );
		g_pText[1]->SetSize ( 3.5f );
		g_pText[1]->SetColor ( 0,0,0,192 );
		g_pText[2]->SetPosition ( 50, 89.0f );
		g_pText[2]->SetSize ( 3.5f );
		g_pText[2]->SetColor ( 0,0,0,192 );
		g_pText[3]->SetPosition ( 50, 97.0f );
		g_pText[3]->SetSize ( 2.5f );
		g_pText[3]->SetColor ( 0,0,0,128 );
		g_pText[4]->SetPosition ( 50, 10.0f );
		g_pText[4]->SetSize ( 3.8f );
		g_pText[4]->SetColor ( 0,0,0,128 );

		UpdateInterpreterAspect();
	}

	agk::SetDebugCallback( &CallbackOnAppDebugInfo );
	agk::SetWarningCallback( &CallbackOnAppWarning );
	agk::SetErrorCallback( &CallbackOnAppError );
}

void app::DrawBackground()
{
	if ( agk::GetDeviceWidth() != g_iPrevWidth || agk::GetDeviceHeight() != g_iPrevHeight )
	{
		UpdateInterpreterAspect();
	}

	if ( g_pAGKBackdrop.pSprite ) g_pAGKBackdrop.pSprite->Draw();
	if ( g_pAGKBackdropLogo.pSprite ) g_pAGKBackdropLogo.pSprite->Draw();

	if ( g_pAGKBackdropSpinner.pSprite )
	{
		static int iCycleCounter = 0;
		g_pAGKBackdropSpinner.pSprite->SetAngle ( (float) iCycleCounter );
		g_pAGKBackdropSpinner.pSprite->Draw();
		iCycleCounter++;
		iCycleCounter %= 360;
	}
	
		
#ifndef AGKIOS
	// draw power off button
	if ( g_pAGKBackdropPower.pSprite ) g_pAGKBackdropPower.pSprite->Draw();
	if ( agk::GetPointerPressed() == 1 )
	{
		if ( g_pAGKBackdropPower.pSprite->GetHitTest( agk::GetPointerX(), agk::GetPointerY() ) )
		{
			// quit app
			if ( m_DeviceBroadcaster.IsRunning() ) 
			{
				m_DeviceBroadcaster.Stop();
				m_DeviceBroadcaster.Join();
			}
			if ( m_ConnectionListener.IsRunning() ) 
			{
				m_ConnectionListener.Stop();
				m_ConnectionListener.Join();
			}
			AppQuit();
		}
	}
#endif
}

void app::DrawText( const char* text1, const char* text2, const char* text3 )
{
	if ( g_pText[0] )
	{
		g_pText[0]->SetString ( text1 );
		g_pText[0]->Draw();
		g_pText[1]->SetString ( text2 );
		g_pText[1]->Draw();
		g_pText[2]->SetString ( text3 );
		g_pText[2]->Draw();
		uString temp("Built ");
		temp.Append( __DATE__ );
		g_pText[3]->SetString ( temp );
		g_pText[3]->Draw();

		static float IP_time = 2.5f;
		IP_time += agk::GetFrameTime();
		//if ( (m_sIP.GetLength() == 0 || m_sIP.CompareTo( "<None>" ) == 0) && IP_time > 1 )
		if ( IP_time > 3 ) // keep updating the IP
		{
			IP_time = 0;
			char *szIP = agk::GetDeviceIP();
			if ( szIP && *szIP ) m_sIP.SetStr( szIP );
			else m_sIP.SetStrUTF8( "<None>" );
			
			if ( szIP ) delete [] szIP;
		}

		temp.SetStrUTF8( "Device IP: " );
		temp.Append( m_sIP.GetStr() );
		g_pText[4]->SetString ( temp );
		g_pText[4]->Draw();
	}
}

void app::CheckConnection()
{
	if ( !m_pConnection ) return;

	if ( m_iPingResponded )
	{
		if ( m_fPingStart + 2 < agk::GetRunTime() )
		{
			m_iPingResponded = 0;
			m_fPingStart = agk::GetRunTime();
			m_pConnection->SendUInt( 1 ); // client ping
			m_pConnection->Flush();
		}
	}
	else
	{
		if ( m_fPingStart + 10 < agk::GetRunTime() )
		{
			// server hasn't responded in 10 seconds, assume disconnected.
			if ( !m_pConnection->GetDisconnected() ) m_pConnection->ForceClose();
		}
	}
}

void app::CheckMessages()
{
	if ( !m_pConnection ) return;
	if ( m_pConnection->GetDisconnected() ) return;
	if ( !m_pConnection->IsConnected() ) return;

	while ( m_pConnection->GetBytes() >= 4 )
	{
		UINT type = m_pConnection->RecvUInt();
		if ( m_pConnection->GetDisconnected() ) break;

		switch( type )
		{
			case 0: // ping
			{
				m_pConnection->SendUInt( 0 ); // respond
				break;
			}

			case 1: // client ping response
			{
				m_iPingResponded = 1;
				break;
			}

			case 2: // run
			{
				//agk::Warning("Run");
				if ( m_iAppControlStage == APP_WAITING2 )
				{
					agk::SetWindowAllowResize(0);
					m_iDebugMode = 0;
					m_iStepMode = 0;
					m_iAppControlStage = APP_RUNNING;
				}
				break;
			}

			case 3: // stop
			{
				//agk::Warning("Stopped");
				if ( m_iAppControlStage == APP_RUNNING || m_iAppControlStage == APP_PAUSED )
				{
					AppFinished();
				}
				break;
			}

			case 4: // pause
			{
				if ( m_iAppControlStage == APP_RUNNING )
				{
					AppPause();
					m_iAppControlStage = APP_PAUSED;
					App.OnAppBreak( m_sProgram.GetCurrentIncludeFile(), m_sProgram.GetCurrentLineNum() );
				}
				break;
			}

			case 5: // add breakpoint
			{
				//int ID = m_pConnection->RecvInt();
				uString filename;
				m_pConnection->RecvString( filename );
				int line = m_pConnection->RecvInt();
				//uString info;
				//info.Format( "Breakpoint %d set in %s at line %d", ID, filename.GetStr(), line );
				//agk::Warning( info );
				m_sProgram.AddBreakpoint( filename, line );
				break;
			}

			case 6: // remove breakpoint
			{
				//int ID = m_pConnection->RecvInt();
				uString filename;
				m_pConnection->RecvString( filename );
				int line = m_pConnection->RecvInt();
				//uString info;
				//info.Format( "Breakpoint %d removed in %s at line %d", ID, filename.GetStr(), line );
				//agk::Warning( info );
				m_sProgram.RemoveBreakpoint( filename, line );
				break;
			}

			case 7: // continue
			{
				if ( m_iAppControlStage == APP_PAUSED )
				{
					m_iStepMode = 0;
					AppResume();
					m_iAppControlStage = APP_RUNNING;
				}
				break;
			}

			case 8: // debug
			{
				if ( m_iAppControlStage == APP_WAITING2 )
				{
					if ( m_sProgram.GetDebugMode() == 0 )
					{
						agk::Warning( "Bytecode file does not having any debugging information, variables and call stack will be unavailable" );
					}

					agk::SetWindowAllowResize(0);
					m_iDebugMode = 1;
					m_iStepMode = 0;
					m_iAppControlStage = APP_RUNNING;
				}
				break;
			}

			case 9: // step in
			{
				if ( m_iAppControlStage == APP_PAUSED )
				{
					m_iStepMode = 1;
					AppResume();
					m_iAppControlStage = APP_RUNNING;
				}
				break;
			}

			case 10: // step over
			{
				if ( m_iAppControlStage == APP_PAUSED )
				{
					m_iStepMode = 2;
					AppResume();
					m_iAppControlStage = APP_RUNNING;
				}
				break;
			}

			case 11: // step out
			{
				if ( m_iAppControlStage == APP_PAUSED )
				{
					m_iStepMode = 3;
					AppResume();
					m_iAppControlStage = APP_RUNNING;
				}
				break;
			}

			case 12: // add variable watch
			{
				uString var;
				m_pConnection->RecvString( var );
				//uString info;
				//info.Format( "Variable watch added for %s", var.GetStr() );
				//agk::Warning( info );
				AGKVariableWatch *pVar = m_sProgram.AddVariableWatch( var );
				if ( m_iAppControlStage == APP_PAUSED ) m_sProgram.PrintWatchVariables( pVar );
				break;
			}

			case 13: // remove variable watch
			{
				uString var;
				m_pConnection->RecvString( var );
				//uString info;
				//info.Format( "Variable watch removed for", var.GetStr() );
				//agk::Warning( info );
				m_sProgram.RemoveVariableWatch( var );
				break;
			}

			case 14: // set frame
			{
				int iFrame = m_pConnection->RecvInt();
				m_sProgram.SetCallStackFrame( iFrame );
				if ( m_iAppControlStage == APP_PAUSED )
				{
					m_sProgram.PrintWatchVariables();
				}
				break;
			}

			case 99: // disconnected
			{
				m_pConnection->Close();
				//agk::Warning("Disconnected");
				if ( m_iAppControlStage != APP_RUNNING )
				{
					AppFinished();
				}
				break;
			}
		}

		m_pConnection->Flush();
	}
}

void app::OnAppDebugInfo( const char *szType, const char *szData )
{
	if ( !szType || !szData ) return;
	if ( !m_pConnection ) return;
	if ( m_pConnection->GetDisconnected() ) return;
	if ( !m_pConnection->IsConnected() ) return;

	UINT length = (UINT)strlen( szType );
	if ( length == 0 ) return;

	length = (UINT)strlen( szData );
	if ( length == 0 ) return;

	m_pConnection->SendUInt( 4 );
	m_pConnection->SendString( szType );
	m_pConnection->SendString( szData );
	m_pConnection->Flush();
}

static int warningloop = 0;

void app::OnAppWarning( const char *szMsg )
{
	if ( !szMsg ) return;
	if ( !m_pConnection ) return;
	if ( m_pConnection->GetDisconnected() ) return;
	if ( !m_pConnection->IsConnected() ) return;

	UINT length = (UINT)strlen( szMsg );
	if ( length == 0 ) return;

	// make sure we don't generate a warning as a result of generating a warning
	if ( warningloop > 0 ) return;
	warningloop = 1;

	uString sMsg( szMsg );
	if ( m_iAppControlStage == APP_RUNNING || m_iAppControlStage == APP_PAUSED )
	{
		const char* szInclude = m_sProgram.GetCurrentIncludeFile();
		if ( szInclude && *szInclude )
		{
			sMsg.Append( " in " );
			sMsg.Append( m_sProgram.GetCurrentIncludeFile() );
			sMsg.Append( " at line " );
			sMsg.AppendInt( m_sProgram.GetCurrentLineNum() );
		}
	}

	m_pConnection->SendUInt( 2 );
	m_pConnection->SendString( sMsg );
	m_pConnection->Flush();

	warningloop = 0;
}

void app::OnAppError( const char *szMsg )
{
	if ( !szMsg ) return;
	if ( !m_pConnection ) return;
	if ( m_pConnection->GetDisconnected() ) return;
	if ( !m_pConnection->IsConnected() ) return;

	UINT length = (UINT)strlen( szMsg );
	if ( length == 0 ) return;

	// make sure we don't generate a warning as a result of generating a warning
	if ( warningloop > 0 ) return;
	warningloop = 1;

	uString sMsg( szMsg );
	if ( m_iAppControlStage == APP_RUNNING || m_iAppControlStage == APP_PAUSED )
	{
		const char* szInclude = m_sProgram.GetCurrentIncludeFile();
		if ( szInclude && *szInclude )
		{
			sMsg.Append( " in " );
			sMsg.Append( m_sProgram.GetCurrentIncludeFile() );
			sMsg.Append( " at line " );
			sMsg.AppendInt( m_sProgram.GetCurrentLineNum() );
		}
	}

	m_pConnection->SendUInt( 3 );
	m_pConnection->SendString( sMsg );
	m_pConnection->Flush();

	warningloop = 0;
}

void app::OnAppBreak( const char *szInclude, int line )
{
	if ( !szInclude ) return;
	if ( !m_pConnection ) return;
	if ( m_pConnection->GetDisconnected() ) return;
	if ( !m_pConnection->IsConnected() ) return;

	UINT length = (UINT)strlen( szInclude );
	if ( length == 0 ) return;

	m_pConnection->SendUInt( 5 );
	m_pConnection->SendString( szInclude );
	m_pConnection->SendInt( line );
	m_pConnection->Flush();

	m_sProgram.PrintWatchVariables();
	m_sProgram.PrintCallStack();
}


static int paused = 0;
static int musicpaused = 0;
static int videopaused = 0;

void app::AppPause()
{
	paused = 1;
	if ( agk::GetMusicPlaying() )
	{
		agk::PauseMusic();
		musicpaused = 1;
	}

	if ( agk::GetVideoPlaying() )
	{
		agk::PauseVideo();
		videopaused = 1;
	}

	agk::StopSound(0);
}

void app::AppResume()
{
	if ( paused )
	{
		if ( musicpaused ) agk::ResumeMusic();
		if ( videopaused ) agk::PlayVideo();
		agk::Resumed();
	}

	paused = 0;
	musicpaused = 0;
}

// Main loop, called every frame
void app::Loop ( void )
{
    if ( agk::GetResumed2() )
    {
        m_iPingResponded = 1;
        m_fPingStart = agk::GetRunTime();
    }
    
	switch( m_iAppControlStage )
	{
		case APP_IDLE: 
		{
			DrawBackground();
			DrawText( "IDLE", "To exit a running app, simply press the", "top of the screen for 5 seconds" );
			
			agk::Sleep( 30 );
			agk::Swap();
			break;
		}

		case APP_NETWORK_SETUP:
		{
			static float timer_start = 0;
			if ( timer_start == 0 ) timer_start = agk::GetRunTime();

			if ( m_pConnection ) delete m_pConnection;
			m_pConnection = 0;

			// set up the networking to wait for a connection
			if ( !m_DeviceBroadcaster.IsRunning() )
			{
				AGKPacket packet;
                packet.AddUInt( m_iConnectionPort );
                m_DeviceBroadcaster.SetData( m_iBroadcastPort, &packet, 1000 );
                m_DeviceBroadcaster.Start();
			}

			if ( !m_ConnectionListener.IsRunning() )
			{
				char *szIP = agk::GetDeviceIP();
				m_sListeningIP.SetStr( szIP );
				delete [] szIP;
                //m_ConnectionListener.AcceptConnections( m_sListeningIP.GetStr(), m_iConnectionPort );
                //if ( !m_ConnectionListener.IsRunning() )
                {
                    m_ConnectionListener.AcceptConnections( m_iConnectionPort );
                }

            }
            
            // only proceed when both are setup and running, sometimes fails first time on iOS after a disconnect
            if ( m_DeviceBroadcaster.IsRunning() && m_ConnectionListener.IsRunning() )
            {
                m_iAppControlStage = APP_WAITING;
				timer_start = 0;
            }
			else
			{
				if ( agk::GetRunTime() - timer_start > 5 )
				{
					m_iAppControlStage = APP_NETWORK_ERROR;
					timer_start = 0;
				}
			}

			DrawBackground();
			DrawText( "INITIALISING", "To exit a running app, simply press the", "top of the screen for 5 seconds" );
			
			agk::Sleep( 30 );
			agk::Swap();
			break;
		}

		case APP_WAITING:
		{
			static float m_fIPCheckTime = 0;
			m_fIPCheckTime += agk::GetFrameTime();
			if ( m_fIPCheckTime > 1 )
			{
				m_fIPCheckTime = 0;
				char *szIP = agk::GetDeviceIP();
				if ( m_sListeningIP.CompareTo( szIP ) != 0 )
				{
					m_DeviceBroadcaster.Stop();
					m_ConnectionListener.Stop();
					m_DeviceBroadcaster.Join();
					m_ConnectionListener.Join();
					m_iAppControlStage = APP_NETWORK_SETUP;
					delete [] szIP;
					break;
				}
				delete [] szIP;
			}

			m_pConnection = m_ConnectionListener.GetNewConnection();
			if ( m_pConnection )
			{
				m_iPingResponded = 1;
				m_fPingStart = agk::GetRunTime();

				m_DeviceBroadcaster.Stop();
				m_ConnectionListener.Stop();
				while ( 1 )
				{
					// clean up any other connections that came in as well, we only want one
					AGKSocket *temp = m_ConnectionListener.GetNewConnection();
					if ( !temp ) break;
					delete temp;
				}
				m_iAppControlStage = APP_CONNECTED;
				break;
			}

			// draw all graphics
			DrawBackground();
			DrawText( "UNCONNECTED", "To exit a running app, simply press the", "top of the screen for 5 seconds" );
			
			agk::Sleep( 30 );
			agk::Swap();
			break;
		}

		case APP_CONNECTED:
		{
			if ( m_pConnection->GetBytes() > 0 )
			{
				uString sName;
				m_pConnection->RecvString( sName );
				if ( m_pConnection->GetDisconnected() || sName.GetLength() == 0 )
				{
					delete m_pConnection;
					m_pConnection = 0;
					m_iAppControlStage = APP_NETWORK_SETUP;
				}

				agk::SetAppName( sName );
				m_FileReceiver.SetSender( m_pConnection, false );
				m_FileReceiver.Start();
				m_iAppControlStage = APP_RECEIVING;
				break;
			}

			if ( m_pConnection->GetDisconnected() )
			{
				delete m_pConnection;
				m_pConnection = 0;
				m_iAppControlStage = APP_NETWORK_SETUP;
			}

			// draw all graphics
			DrawBackground();
			DrawText( "CONNECTED", "To exit a running app, simply press the", "top of the screen for 5 seconds" );
			
			agk::Sleep( 30 );
			agk::Swap();
			break;
		}

		case APP_RECEIVING:
		{
			if ( m_FileReceiver.IsFinished() )
			{
				m_FileReceiver.Reset();
				if ( m_FileReceiver.IsSuccess() ) 
				{
					if ( !m_sProgram.LoadBytecode( "bytecode.byc" ) )
					{
						m_iAppControlStage = APP_ERROR;
					}
                    else
                    {
                        m_iPingResponded = 1;
                        m_fPingStart = agk::GetRunTime();
                        m_iAppControlStage = APP_WAITING2;
                    }
				}
				else
				{
					m_iAppControlStage = APP_ERROR;
				}
				break;
			}

			char pProgressText[128];
            sprintf ( pProgressText, "%d%%", (int)m_FileReceiver.GetProgress() );

			// draw all graphics
			DrawBackground();
			DrawText( "RECEIVING APP", pProgressText, "" );
			
			agk::Sleep( 30 );
			agk::Swap();
			break;
		}

		case APP_WAITING2:
		{
            CheckMessages();
			CheckConnection();

			if ( !m_pConnection ) 
			{
				m_iAppControlStage = APP_NETWORK_SETUP;
				break;
			}

			if ( m_pConnection->GetDisconnected() )
			{
				delete m_pConnection;
				m_pConnection = 0;
				m_iAppControlStage = APP_NETWORK_SETUP;
				break;
			}

			// draw all graphics
			DrawBackground();
			DrawText( "WAITING", "To exit a running app, simply press the", "top of the screen for 5 seconds" );
			
			agk::Sleep( 30 );
			agk::Swap();
			break;
		}

		case APP_PAUSED:
		case APP_RUNNING:
		{
            if ( m_pConnection && !m_pConnection->GetDisconnected() )
			{
				CheckMessages();
				CheckConnection();

				if ( m_pConnection->GetDisconnected() )
				{
					delete m_pConnection;
					m_pConnection = 0;
				}
			}

			if ( m_iAppControlStage == APP_RUNNING )
			{
				try
				{
					int result = 0;
					if ( m_iDebugMode == 0 ) result = m_sProgram.RunProgram();
					else result = m_sProgram.RunProgramDebug( m_iStepMode );
					if ( result == 1 ) 
					{
						if ( m_iStandAlone == 0 ) AppFinished();
						else m_iAppControlStage = APP_RUNTIME_ERROR;
						break;
					}
					else if ( result == 2 )
					{
						AppPause();
						m_iAppControlStage = APP_PAUSED;
					}
				}
				catch(...)
				{
					uString err; agk::GetLastError(err);
					err.Prepend( "Error: " );
					//agk::Message( err.GetStr() );
					m_sProgram.RuntimeError( err.GetStr() );
					if ( m_iStandAlone == 0 ) AppFinished();
					else m_iAppControlStage = APP_RUNTIME_ERROR;
					break;
				}
			}
			else agk::Sleep( 30 );

			// check joystick exit
			if ( m_iStandAlone == 0 && agk::GetRawJoystickExists(1) == 1 ) 
			{
				if ( agk::GetRawJoystickButtonState(1,9) == 1 && agk::GetRawJoystickButtonState(1,10) == 1 
				  && agk::GetRawJoystickButtonState(1,5) == 1 && agk::GetRawJoystickButtonState(1,6) == 1 )
				{
					AppFinished();
					break;
				}
			}

			// check top screen 5 second press
			if ( m_iStandAlone == 0 )
			{
				static float pointerStartX = 0;
				static float pointerStartY = 0;
				static float pointerStartTime = 0;
				if ( agk::GetPointerPressed() == 1 )
				{
					pointerStartX = agk::GetPointerX();
					pointerStartY = agk::GetPointerY();
					pointerStartTime = agk::Timer();
				}

				// check if we should increase exit timer
				if ( pointerStartTime > 0 )
				{
					if ( agk::GetPointerState()==1 && agk::GetPointerY()<agk::GetVirtualHeight()/12.5f )
					{
						float fDiffX = agk::GetPointerX() - pointerStartX;
						float fDiffY = agk::GetPointerY() - pointerStartY;
						float fDiffDistance = fDiffX*fDiffX + fDiffY*fDiffY;

						float smallDist = agk::GetVirtualWidth()/20.0f;
						
						if ( fDiffDistance > smallDist*smallDist )
						{
							pointerStartTime = 0;
						}
						else
						{
							// timer based exit, 4 seconds
							if ( agk::Timer() - pointerStartTime > 4 )
							{
								// user has held down a while, wants to leave
								AppFinished();
								break;
							}
						}
					}
					else pointerStartTime = 0;
				}
			}

			break;
		}

		case APP_ERROR:
		{
			CheckMessages();
			CheckConnection();

			if ( !m_pConnection ) 
			{
				m_iAppControlStage = APP_NETWORK_SETUP;
				break;
			}

			if ( m_pConnection->GetDisconnected() )
			{
				delete m_pConnection;
				m_pConnection = 0;
				m_iAppControlStage = APP_NETWORK_SETUP;
				break;
			}
			
			// draw all graphics
			DrawBackground();
			DrawText( "ERROR", "Failed to receive app, stop the", "broadcast and try again" );
			
			agk::Sleep( 30 );
			agk::Swap();
			break;
		}

		case APP_NETWORK_ERROR:
		{
			// draw all graphics
			DrawBackground();
			DrawText( "NETWORK ERROR", "Failed to run networking code", "a device restart may be required" );
			
			agk::Sleep( 30 );
			agk::Swap();
			break;
		}

		case APP_RUNTIME_ERROR:
		{
			static int state = agk::GetPointerState();
			if ( agk::GetPointerPressed() ) 
			{
				if ( state == 0 ) AppFinished();
				state = 0;
			}

			agk::Print( "Tap/click the screen to exit" );
			agk::Render();
			
			agk::Sleep( 30 );
			agk::Swap();
			break;
		}
	}
}

// End app, called once at the end
void app::End ( void )
{
	if ( m_ConnectionListener.IsRunning() ) 
	{
		m_ConnectionListener.Stop();
		m_ConnectionListener.Join();
	}

	if ( m_DeviceBroadcaster.IsRunning() ) 
	{
		m_DeviceBroadcaster.Stop();
		m_DeviceBroadcaster.Join();
	}	
	
	if ( m_pConnection ) delete m_pConnection;
	m_pConnection = 0;
}

void app::AppClose ( void )
{
	// master reset everything
	agk::SetClearColor ( 0, 0, 0 );
	agk::SetViewOffset ( 0, 0 );
	agk::StopTextInput();
	agk::DeleteAdvert();
	agk::MasterReset();
    agk::Update();
	agk::SetWindowAllowResize(1);

	m_sProgram.CleanUp();
}

void app::AppReload ( void )
{
	AppClose();
	m_sProgram.LoadBytecode( "bytecode.byc" );
}

void app::AppFinished()
{
	paused = 0;
	if ( m_iStandAlone ) AppQuit();
	else
	{
		if ( m_pConnection && !m_pConnection->GetDisconnected() )
		{
			AppReload();
			UpdateInterpreterAspect();
			m_iAppControlStage = APP_WAITING2;
		}
		else
		{
			AppClose();
			UpdateInterpreterAspect();
			agk::RestoreWriteDir();
			m_iAppControlStage = APP_NETWORK_SETUP;
		}
	}
}

void app::AppSplash()
{
#ifdef FREEVERSION
    agk::MasterReset();
    
    if ( !App.g_pAGKBackdrop.pImage ) App.g_pAGKBackdrop.pImage = new cImage ( "/interpreter-backdrop.png" );
    if ( !App.g_pAGKBackdrop.pSprite ) App.g_pAGKBackdrop.pSprite = new cSprite ( App.g_pAGKBackdrop.pImage );
    App.g_pAGKBackdrop.pSprite->FixToScreen ( 1 );
    if ( !App.g_pAGKBackdropSpinner.pImage ) App.g_pAGKBackdropSpinner.pImage = new cImage ( "/interpreter-spinner.png" );
    if ( !App.g_pAGKBackdropSpinner.pSprite ) App.g_pAGKBackdropSpinner.pSprite = new cSprite ( App.g_pAGKBackdropSpinner.pImage );
    App.g_pAGKBackdropSpinner.pSprite->SetAlpha ( 16 );
    App.g_pAGKBackdropSpinner.pSprite->FixToScreen ( 1 );
    if ( !App.g_pAGKBackdropLogo.pImage ) App.g_pAGKBackdropLogo.pImage = new cImage ( "/interpreter-logo.png" );
    if ( !App.g_pAGKBackdropLogo.pSprite ) App.g_pAGKBackdropLogo.pSprite = new cSprite ( App.g_pAGKBackdropLogo.pImage );
    App.g_pAGKBackdropLogo.pSprite->FixToScreen ( 1 );
    
    App.UpdateInterpreterAspect();
    
    // agk::MasterReset();
    float dwTimerEnd = agk::Timer() + 4;
    AGK::cText* pSlashText[3];
    cImage* pArial = new cImage( "/Arial.png" );
    
    
    for ( int t=0; t<3; t++ )
    {
        pSlashText[t] = new AGK::cText ();
        pSlashText[t]->SetAlignment ( 1 );
        pSlashText[t]->SetFontImage( pArial );
        pSlashText[t]->FixToScreen ( 1 );
        pSlashText[t]->SetPosition ( 50, 10+(t*75.0f) );
        pSlashText[t]->SetColor ( 32, 32, 32, 255 );
        pSlashText[t]->SetSize ( 6.0f-(t*2.5f) );
        if ( t==0 ) pSlashText[t]->SetString (  "TRIAL APPLICATION" );
        if ( t==1 ) pSlashText[t]->SetString (  "Not for distribution\nCreated with AGK - www.appgamekit.com" );
        if ( t==2 ) pSlashText[t]->SetString (  "4" );
    }
    
    
    int iCycleCounter = 0;
    int iFadeIn = 0;
    char szTimeLeft[32];
    while ( agk::Timer() < dwTimerEnd )
    {
        int timeleft = floor(dwTimerEnd - agk::Timer());
        sprintf( szTimeLeft, "%d", timeleft+1 );
        agk::ClearScreen();
        if ( App.g_pAGKBackdrop.pSprite ) App.g_pAGKBackdrop.pSprite->Draw();
        if ( App.g_pAGKBackdropSpinner.pSprite )
        {
            App.g_pAGKBackdropSpinner.pSprite->SetAngle ( (float)(iCycleCounter % 360) );
            App.g_pAGKBackdropSpinner.pSprite->Draw();
            iCycleCounter+=2;
        }
        if ( App.g_pAGKBackdropLogo.pSprite )
        {
            App.g_pAGKBackdropLogo.pSprite->SetAlpha ( iFadeIn );
            App.g_pAGKBackdropLogo.pSprite->Draw();
        }
        pSlashText[0]->SetAlpha((int)(iFadeIn*0.75f));
        pSlashText[0]->Draw();
        pSlashText[1]->SetAlpha((int)(iFadeIn*0.75f));
        pSlashText[1]->Draw();
        pSlashText[2]->SetPosition ( 50, 20 );
        pSlashText[2]->SetSize ( 5 );
        pSlashText[2]->SetString( szTimeLeft );
        pSlashText[2]->SetAlpha((int)(iFadeIn*0.75f));
        pSlashText[2]->Draw();
        iFadeIn+=8;
        if ( iFadeIn>255 ) iFadeIn = 255;
        agk::Swap();
    }
    delete pSlashText[0];
    delete pSlashText[1];
    delete pSlashText[2];
#endif
}

void app::AppQuit()
{
    AppSplash();

	PlatformAppQuit();
}
