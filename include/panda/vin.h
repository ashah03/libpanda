/*
 Author: Matt Bunting
 Copyright (c) 2020 Arizona Board of Regents
 All rights reserved.

 Permission is hereby granted, without written agreement and without
 license or royalty fees, to use, copy, modify, and distribute this
 software and its documentation for any purpose, provided that the
 above copyright notice and the following two paragraphs appear in
 all copies of this software.

 IN NO EVENT SHALL THE ARIZONA BOARD OF REGENTS BE LIABLE TO ANY PARTY
 FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN
 IF THE UNIVERSITY OF CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF
 SUCH DAMAGE.

 THE ARIZONA BOARD OF REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE PROVIDED HEREUNDER
 IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION
 TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

 */

#ifndef VIN_H
#define VIN_H


//#include <unistd.h>	// usleep()
//#include <cstring> // memcpy

//#include "panda.h"
//#include "panda/obd-pid.h"

//#define CAN_FRAME_TYPE_MASK (0xF0)
//
namespace Panda {
//
//enum CanFrameTypes {
//	CAN_FRAME_SINGLE = 0x00,
//	CAN_FRAME_FIRST = 0x01,
//	CAN_FRAME_CONSECUTIVE = 0x02,
//	CAN_FRAME_FLOW_CONTROL = 0x03
//};

enum VehicleManufacturer {
	VEHICLE_MANUFACTURE_OTHER,
	VEHICLE_MANUFACTURE_TOYOTA,
	VEHICLE_MANUFACTURE_HONDA,
	VEHICLE_MANUFACTURE_NISSAN,
	VEHICLE_MANUFACTURE_GM
};

enum VehicleRegion {
	VEHICLE_REGION_OTHER,
	VEHICLE_REGION_USA,
	VEHICLE_REGION_CANADA,
	VEHICLE_REGION_JAPAN
};


enum VehicleModel {
	VEHICLE_MODEL_OTHER,
	VEHICLE_MODEL_TOYOTA_RAV4,
	VEHICLE_MODEL_HONDA_PILOT,
	VEHICLE_MODEL_NISSAN_ROGUE,
	VEHICLE_MODEL_GM_CADIALLC_XT5
};

/*! \brief Retreives the year, only for VIN in vehicles made after 2000
	\param vin The VIN
	\return The vehicle's year
 */
int vinToYear( const unsigned char* vin);


/*! \brief Retreives the manufacturer, but expects vehicles most liekly to be found in USA or maybe Japan
	see https://en.wikibooks.org/wiki/Vehicle_Identification_Numbers_(VIN_codes)/World_Manufacturer_Identifier_(WMI)
	\param vin The VIN
	\return The vehicle's manufacturer, as only recognizable by libpanda
 */
VehicleManufacturer vinToManufacturer( const unsigned char* vin );


/*! \brief Retreives the region of manufacturer, but expects vehicles most liekly to be found in USA or maybe Japan
	\param vin The VIN
	\return The vehicle's manufacturer region, as only recognizable by libpanda
 */
VehicleRegion vinToRegion( const unsigned char* vin );

/*! \brief Retreives the model (and thus manufacture).  This is emprically based on the 3rd digit of the VIN with a super small sample of vehicles
	see https://en.wikibooks.org/wiki/Vehicle_Identification_Numbers_(VIN_codes)/World_Manufacturer_Identifier_(WMI)
	\param vin The VIN
	\return The vehicle's model, as only recognizable by libpanda
 */
VehicleModel vinToModel( const unsigned char* vin );

const char* vehicleManufacturerToString( const VehicleManufacturer vm );
const char* vehicleModelToString( const VehicleModel vm );
const char* vehicleRegionToString( const VehicleRegion vr );

void printVin( const unsigned char* vin);

/*
class VinReader : public CanListener {
public:
	
	VinReader();
	
	// Check this before using system time
	bool vinRead();
	const char* vinResult();
	void setPandaHandler(Handler& handler);
	void requestVin();
	
	//Will save the VIN to a file when discovered:
	
	void saveToFile(const char* filename);
	
private:
	bool vinHasBeenRead;
	Handler *mPandaHandler;
	
	int responseLength;
	unsigned int assignedId;
	char vin[17];
	int vinLength; // this should always be 17
	
	bool shouldSaveToFile;
	FILE* file;
	
	void sendFlowControl();
	void newDataNotification( CanFrame* canFrame );
};
*/
}

#endif
