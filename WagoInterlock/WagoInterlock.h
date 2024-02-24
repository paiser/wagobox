/*----- PROTECTED REGION ID(WagoInterlock.h) ENABLED START -----*/
//=============================================================================
//
// file :        WagoInterlock.h
//
// description : Include file for the WagoInterlock class
//
// project :     
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
// $Revision: 1.4 $
// $Date: 2013-09-30 16:15:01 $
//
// $HeadURL:  $
//
//=============================================================================
//                This file is generated by POGO
//        (Program Obviously used to Generate tango Object)
//=============================================================================


#ifndef WagoInterlock_H
#define WagoInterlock_H

#include <tango.h>

/*!Structure describing interlock channel cnfiguration
 *
 */
typedef struct channel_t{
	std::string name; //!Name of related wago attribute
	std::string attributeName; //!Name of interlock attribute
	unsigned short logicalChannel;
	unsigned short type; 	// The channel type <chtype> is a mandatory two-letter code that indicates
							// the type of I/O channel. At least the following codes should be valid:
							// IB (input bit), OB (output bit), IW (input word), OW (output word),
							// TC (thermocouple), IV (input voltage), OV (output voltage).
	float	threshold_low;
	float	threshold_high;
	unsigned short flags;
	unsigned short channelProcImgOffset;
}channel_t;

/*----- PROTECTED REGION END -----*/	//	WagoInterlock.h

#ifdef TANGO_LOG
	// cppTango after c934adea (Merge branch 'remove-cout-definition' into 'main', 2022-05-23)
	// nothing to do
#else
	// cppTango 9.3-backports and older
	#define TANGO_LOG       cout
	#define TANGO_LOG_INFO  cout2
	#define TANGO_LOG_DEBUG cout3
#endif // TANGO_LOG

/**
 *  WagoInterlock class description:
 *    Class representing interlocks implemented using Wago
 */


namespace WagoInterlock_ns
{
/*----- PROTECTED REGION ID(WagoInterlock::Additional Class Declarations) ENABLED START -----*/

//	Additional Class Declarations
class InterlockAttribute; //forward declaration
class WagoAttribute;

/*! Structure for mapping interlock dynamic attributes into corresponding wago attributes
 *
 */
typedef struct wagoAttrMapping_t{
	WagoAttribute *wAttr; //!Pointer to corresponding wago attribute interface class.
	int position;//!If corresponding wago attribute is scallar attribute this indicates the offset at which requested value is kept.
}wagoAttrMapping_t;

/*----- PROTECTED REGION END -----*/	//	WagoInterlock::Additional Class Declarations

class WagoInterlock : public TANGO_BASE_CLASS
{

/*----- PROTECTED REGION ID(WagoInterlock::Data Members) ENABLED START -----*/

//	Add your own data members
private:
	Tango::DeviceProxy* wagoDeviceProxyObj;	// Interface object to Wago server.
	int interlockInstance;	//Instance number of interlock within Wago box.
	unsigned short modeFlags;	// Numerical value of interlock flags.
	unsigned short outputChannel;	// Offset within Wago process image representing ouput relay of interlock.
/*----- PROTECTED REGION END -----*/	//	WagoInterlock::Data Members

//	Device property data members
public:
	//	config:	Configuration of the interlock
	std::vector<std::string>	config;
	//	name:	Interlock name
	std::string	name;
	//	flags:	Interlock flags
	std::string	flags;
	//	outRelay:	output relay name
	std::string	outRelay;
	//	wagoDevice:	Corresponding wago device tango server name
	std::string	wagoDevice;
	//	DataPeriod:	Data refresh period
	Tango::DevUShort	dataPeriod;
	//	StatusPeriod:	Status refresh period
	Tango::DevUShort	statusPeriod;

	bool	mandatoryNotDefined;


//	Constructors and destructors
public:
	/**
	 * Constructs a newly device object.
	 *
	 *	@param cl	Class.
	 *	@param s 	Device Name
	 */
	WagoInterlock(Tango::DeviceClass *cl,std::string &s);
	/**
	 * Constructs a newly device object.
	 *
	 *	@param cl	Class.
	 *	@param s 	Device Name
	 */
	WagoInterlock(Tango::DeviceClass *cl,const char *s);
	/**
	 * Constructs a newly device object.
	 *
	 *	@param cl	Class.
	 *	@param s 	Device name
	 *	@param d	Device description.
	 */
	WagoInterlock(Tango::DeviceClass *cl,const char *s,const char *d);
	/**
	 * The device object destructor.
	 */
	~WagoInterlock();


//	Miscellaneous methods
public:
	/*
	 *	will be called at device destruction or at init command.
	 */
	void delete_device();
	/*
	 *	Initialize the device
	 */
	virtual void init_device();
	/*
	 *	Read the device properties from database
	 */
	void get_device_property();
	/*
	 *	Always executed method before execution command method.
	 */
	virtual void always_executed_hook();

	/*
	 *	Check if mandatory property has been set
	 */
	 void check_mandatory_property(Tango::DbDatum &class_prop, Tango::DbDatum &dev_prop);

//	Attribute methods
public:
	//--------------------------------------------------------
	/*
	 *	Method      : WagoInterlock::read_attr_hardware()
	 * Description:  Hardware acquisition for attributes.
	 */
	//--------------------------------------------------------
	virtual void read_attr_hardware(std::vector<long> &attr_list);


	//--------------------------------------------------------
	/**
	 *	Method      : WagoInterlock::add_dynamic_attributes()
	 * Description:  Add dynamic attributes if any.
	 */
	//--------------------------------------------------------
	void add_dynamic_attributes();




//	Command related methods
public:
	/**
	 *	Command State related method
	 * Description:  This command gets the device state (stored in its device_state data member) and returns it to the caller.
	 *
	 *	@returns Device state
	 */
	virtual Tango::DevState dev_state();
	/**
	 *	Command UploadConfig related method
	 * Description:  Uploads configuration to controller
	 *
	 */
	virtual void upload_config();
	virtual bool is_UploadConfig_allowed(const CORBA::Any &any);
	/**
	 *	Command Reset related method
	 * Description:  Resets the instance controller (in case of sticky channels)
	 *
	 */
	virtual void reset();
	virtual bool is_Reset_allowed(const CORBA::Any &any);


	//--------------------------------------------------------
	/**
	 *	Method      : WagoInterlock::add_dynamic_commands()
	 * Description:  Add dynamic commands if any.
	 */
	//--------------------------------------------------------
	void add_dynamic_commands();

/*----- PROTECTED REGION ID(WagoInterlock::Additional Method prototypes) ENABLED START -----*/
	virtual void read_attr( Tango::Attribute &att);
	bool is_read_attribute_allowed(Tango::AttReqType ty);
	void UpdateAttributes();
//	Additional Method prototypes

protected:

	__useconds_t lastDataCallT;	// Timestamp of last data update.
	__useconds_t lastStatusCallT;	// Timestamp of last status update.

	std::map<std::string,wagoAttrMapping_t> attributeMapping; // Vector of mappings of interlock attributes into wago attributes representing channels
													// of interlock.

	std::vector<channel_t> processedConfiguration;	// Vector holding configuration structures for each channel of interlock(created according
												// to config property.

	std::vector<WagoAttribute*> channelAttributes;	// Vector of interface objects to wago attributes representing channels of interlock.
	std::vector<InterlockAttribute*> attributes;	// Vector of interlock attribute objects

	int outRelayAttribIndex;			// Index of position within wago attribute(spectrum)
										// of interlock ouput relay. If wago attribute representing
										// output relay is scalar then outRelayAttribIndex = 0.

	WagoAttribute* outRelayWagoAttrib; // Interface object to wago server attribute
									   // representing interlock output relay.

	bool configOK;				// Flag indicating validity of interlock config.
	bool configUploadAllowed; // Flag to indicate if upload of configuration is allowed (true = allowed).

	Tango::DevFailed lastException; // Holder of last catched exception.

	void StatusUpdate();
	void ParseConfiguration();
	void PrepareRelayOutput(std::string outRelayAttribName);
	void PrepareWagoAttributeObjs();
	int CreateDeviceProxy();
	int FindInterlock();
	void RetriveIclkConfig(unsigned short &outChan, unsigned short &mFlags, std::vector<channel_t> &cConf);
	void ValidateIlckConfig(unsigned short &outChan, unsigned short &mFlags, std::vector<channel_t> &cConf);
	unsigned short GetAttrAffsetOffset(std::string wagoAttributeName, unsigned short logicalChannel);

	Tango::DevState GetWagoSrvState();
	bool CheckChannelsValidity();
/*----- PROTECTED REGION END -----*/	//	WagoInterlock::Additional Method prototypes
};

/*----- PROTECTED REGION ID(WagoInterlock::Additional Classes Definitions) ENABLED START -----*/

/*----- PROTECTED REGION END -----*/	//	WagoInterlock::Additional Classes Definitions

}	//	End of namespace

#endif   //	WagoInterlock_H
