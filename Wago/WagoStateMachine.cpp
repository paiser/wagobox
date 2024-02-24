/*----- PROTECTED REGION ID(WagoStateMachine.cpp) ENABLED START -----*/
static const char *RcsId = "$Id: WagoStateMachine.cpp,v 1.6 2013-12-04 15:45:50 kowalisz Exp $";
//=============================================================================
//
// file :        WagoStateMachine.cpp
//
// description : State machine file for the Wago class
//
// project :     Wago controller interface
//
// This file is part of Tango device class.
// 
// Tango is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// Tango is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with Tango.  If not, see <http://www.gnu.org/licenses/>.
// 
// $Author: kowalisz $
//
// $Revision: 1.6 $
// $Date: 2013-12-04 15:45:50 $
//
// $HeadURL:  $
//
//=============================================================================
//                This file is generated by POGO
//        (Program Obviously used to Generate tango Object)
//=============================================================================

#include <Wago.h>

/*----- PROTECTED REGION END -----*/	//	Wago::WagoStateMachine.cpp

//================================================================
//  States  |  Description
//================================================================
//  INIT    |  Server startup is beeing performed.
//  ON      |  Server is operational and running.
//  FAULT   |  Invalid internal server state. Error in communication with device or invalid server configuration.


namespace Wago_ns
{
//=================================================
//		Attributes Allowed Methods
//=================================================


//=================================================
//		Commands Allowed Methods
//=================================================

//--------------------------------------------------------
/**
 *	Method      : Wago::is_DevReadNoCachePhys_allowed()
 * Description:  Execution allowed for DevReadNoCachePhys attribute
 */
//--------------------------------------------------------
bool Wago::is_DevReadNoCachePhys_allowed(TANGO_UNUSED(const CORBA::Any &any))
{
	//	Compare device state with not allowed states.
	if (get_state()==Tango::INIT ||
		get_state()==Tango::FAULT)
	{
	/*----- PROTECTED REGION ID(Wago::DevReadNoCachePhysStateAllowed) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	Wago::DevReadNoCachePhysStateAllowed
		return false;
	}
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : Wago::is_DevWritePhys_allowed()
 * Description:  Execution allowed for DevWritePhys attribute
 */
//--------------------------------------------------------
bool Wago::is_DevWritePhys_allowed(TANGO_UNUSED(const CORBA::Any &any))
{
	//	Compare device state with not allowed states.
	if (get_state()==Tango::INIT ||
		get_state()==Tango::FAULT)
	{
	/*----- PROTECTED REGION ID(Wago::DevWritePhysStateAllowed) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	Wago::DevWritePhysStateAllowed
		return false;
	}
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : Wago::is_DevReadPhys_allowed()
 * Description:  Execution allowed for DevReadPhys attribute
 */
//--------------------------------------------------------
bool Wago::is_DevReadPhys_allowed(TANGO_UNUSED(const CORBA::Any &any))
{
	//	Compare device state with not allowed states.
	if (get_state()==Tango::INIT ||
		get_state()==Tango::FAULT)
	{
	/*----- PROTECTED REGION ID(Wago::DevReadPhysStateAllowed) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	Wago::DevReadPhysStateAllowed
		return false;
	}
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : Wago::is_DevWriteDigi_allowed()
 * Description:  Execution allowed for DevWriteDigi attribute
 */
//--------------------------------------------------------
bool Wago::is_DevWriteDigi_allowed(TANGO_UNUSED(const CORBA::Any &any))
{
	//	Compare device state with not allowed states.
	if (get_state()==Tango::INIT ||
		get_state()==Tango::FAULT)
	{
	/*----- PROTECTED REGION ID(Wago::DevWriteDigiStateAllowed) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	Wago::DevWriteDigiStateAllowed
		return false;
	}
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : Wago::is_DevReadDigi_allowed()
 * Description:  Execution allowed for DevReadDigi attribute
 */
//--------------------------------------------------------
bool Wago::is_DevReadDigi_allowed(TANGO_UNUSED(const CORBA::Any &any))
{
	//	Compare device state with not allowed states.
	if (get_state()==Tango::INIT ||
		get_state()==Tango::FAULT)
	{
	/*----- PROTECTED REGION ID(Wago::DevReadDigiStateAllowed) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	Wago::DevReadDigiStateAllowed
		return false;
	}
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : Wago::is_DevReadNoCacheDigi_allowed()
 * Description:  Execution allowed for DevReadNoCacheDigi attribute
 */
//--------------------------------------------------------
bool Wago::is_DevReadNoCacheDigi_allowed(TANGO_UNUSED(const CORBA::Any &any))
{
	//	Compare device state with not allowed states.
	if (get_state()==Tango::INIT ||
		get_state()==Tango::FAULT)
	{
	/*----- PROTECTED REGION ID(Wago::DevReadNoCacheDigiStateAllowed) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	Wago::DevReadNoCacheDigiStateAllowed
		return false;
	}
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : Wago::is_DevGetKeys_allowed()
 * Description:  Execution allowed for DevGetKeys attribute
 */
//--------------------------------------------------------
bool Wago::is_DevGetKeys_allowed(TANGO_UNUSED(const CORBA::Any &any))
{
	//	Compare device state with not allowed states.
	if (get_state()==Tango::INIT ||
		get_state()==Tango::FAULT)
	{
	/*----- PROTECTED REGION ID(Wago::DevGetKeysStateAllowed) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	Wago::DevGetKeysStateAllowed
		return false;
	}
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : Wago::is_DevKey2Name_allowed()
 * Description:  Execution allowed for DevKey2Name attribute
 */
//--------------------------------------------------------
bool Wago::is_DevKey2Name_allowed(TANGO_UNUSED(const CORBA::Any &any))
{
	//	Compare device state with not allowed states.
	if (get_state()==Tango::INIT ||
		get_state()==Tango::FAULT)
	{
	/*----- PROTECTED REGION ID(Wago::DevKey2NameStateAllowed) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	Wago::DevKey2NameStateAllowed
		return false;
	}
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : Wago::is_DevLog2Hard_allowed()
 * Description:  Execution allowed for DevLog2Hard attribute
 */
//--------------------------------------------------------
bool Wago::is_DevLog2Hard_allowed(TANGO_UNUSED(const CORBA::Any &any))
{
	//	Compare device state with not allowed states.
	if (get_state()==Tango::INIT ||
		get_state()==Tango::FAULT)
	{
	/*----- PROTECTED REGION ID(Wago::DevLog2HardStateAllowed) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	Wago::DevLog2HardStateAllowed
		return false;
	}
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : Wago::is_DevHard2Log_allowed()
 * Description:  Execution allowed for DevHard2Log attribute
 */
//--------------------------------------------------------
bool Wago::is_DevHard2Log_allowed(TANGO_UNUSED(const CORBA::Any &any))
{
	//	Compare device state with not allowed states.
	if (get_state()==Tango::INIT ||
		get_state()==Tango::FAULT)
	{
	/*----- PROTECTED REGION ID(Wago::DevHard2LogStateAllowed) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	Wago::DevHard2LogStateAllowed
		return false;
	}
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : Wago::is_DevName2Key_allowed()
 * Description:  Execution allowed for DevName2Key attribute
 */
//--------------------------------------------------------
bool Wago::is_DevName2Key_allowed(TANGO_UNUSED(const CORBA::Any &any))
{
	//	Compare device state with not allowed states.
	if (get_state()==Tango::INIT ||
		get_state()==Tango::FAULT)
	{
	/*----- PROTECTED REGION ID(Wago::DevName2KeyStateAllowed) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	Wago::DevName2KeyStateAllowed
		return false;
	}
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : Wago::is_DevWcComm_allowed()
 * Description:  Execution allowed for DevWcComm attribute
 */
//--------------------------------------------------------
bool Wago::is_DevWcComm_allowed(TANGO_UNUSED(const CORBA::Any &any))
{
	//	Compare device state with not allowed states.
	if (get_state()==Tango::INIT ||
		get_state()==Tango::FAULT)
	{
	/*----- PROTECTED REGION ID(Wago::DevWcCommStateAllowed) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	Wago::DevWcCommStateAllowed
		return false;
	}
	return true;
}


/*----- PROTECTED REGION ID(Wago::WagoStateAllowed.AdditionalMethods) ENABLED START -----*/
/* clang-format on */
//	Additional Methods
/* clang-format off */
/*----- PROTECTED REGION END -----*/	//	Wago::WagoStateAllowed.AdditionalMethods

}	//	End of namespace
