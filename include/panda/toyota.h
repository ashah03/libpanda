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

#ifndef TOYOTA_H
#define TOYOTA_H


#include "mogi/thread.h"

#include "panda.h"

#define TOYOTA_COMMAND_THREAD_RATE (600.0)
#define TOYOTA_RATE_HEARTBEAT (1.0)	// This is for the panda in general, not Toyota specific
#define TOYOTA_RATE_LKA (1.0)
#define TOYOTA_RATE_TRACK_B (40.0)
#define TOYOTA_RATE_STEER (100.0)
#define TOYOTA_RATE_ACC (30.0)

#define TOYOTA_DECIMATOR_MAX_HEARTBEAT (TOYOTA_COMMAND_THREAD_RATE/TOYOTA_RATE_HEARTBEAT)
#define TOYOTA_DECIMATOR_MAX_LKA (TOYOTA_COMMAND_THREAD_RATE/TOYOTA_RATE_LKA)
#define TOYOTA_DECIMATOR_MAX_TRACK_B (TOYOTA_COMMAND_THREAD_RATE/TOYOTA_RATE_TRACK_B)
#define TOYOTA_DECIMATOR_MAX_STEER (TOYOTA_COMMAND_THREAD_RATE/TOYOTA_RATE_STEER)
#define TOYOTA_DECIMATOR_MAX_ACC (TOYOTA_COMMAND_THREAD_RATE/TOYOTA_RATE_ACC)

namespace Panda {

/*!
 \brief Constructs the LKAS_HUS command that works on a Toyota RAV4
 \param lkaAlert Will invoke a "Please grab steering wheel" notification on the car HUD
 \param leftlane Can be 0-3, and will show different left lane visuals on the HUD
 \param rightlane Can be 0-3, and will show different right lane visuals on the HUD
 \param barrier Will display a barrier on the right and left lanes
 \param twoBeeps Will cause an audible alert in the car.  If called too frequently then it may not trigger
 \param repeatedBeeps Will cause an audible alert in the car.  If called too frequently then it may not trigger
 \return A constructed LKA_HUD CanFrame
 */
CanFrame buildLkasHud(bool lkaAlert, unsigned char leftLane, unsigned char rightLane, bool barrier, bool twoBeeps, bool repeatedBeeps);

/*!
 \brief Constructs the STEERING_LKA command, used for sending steering torque
 \param count This needs to increase by 1 on each send, probably for error checking.
 \param steerTorque In unknown units, but is valid from -1500:1500.
 \param steerRequest Should be set to 1 when a steering torque is sent.  This is shown by CAN reading of the built-in LKA
 \param lkaState Unknown, labeled in the DBC.  Stays 0 based on CAN data.
 \return A constructed STEERING_LKA CanFrame
 */
CanFrame buildSteeringLKA( unsigned char count, int16_t steerTorque, bool steerRequest, unsigned char lkaState );
CanFrame buildACC_CONTROL(double acc, bool permitBraking, bool releaseStandstill, bool miniCar, bool cancelRequest);
CanFrame buildTRACK_B_1(unsigned char count);

CanFrame buildPCM_CRUISE_2(unsigned char SET_SPEED);
CanFrame buildDSU_CRUISE(unsigned char SET_SPEED);

// This computes particular checksums within the CAN message, and is not the CRC for the CAN frmae itself
uint8_t toyotaChecksum(Panda::CanFrame& frame);

// This funciton is helpful for debugging but should either not live in toyota.h or shoul dbe removed:
void printFrame( Panda::CanFrame frame );


class ToyotaHandler : public Mogi::Thread {
private:
	void entryAction(); // Overloaded from Mogi::Thread
	void doAction(); // Overloaded from Mogi::Thread
	
	void sendHeartBeat();
	void sendLka();
	void sendTrackB();
	void sendSteer();
	void sendAcc();
	
	bool heartbeatSteeringPass();
	bool heartbeatAccelerationPass();
	
	Panda::Handler* pandaHandler;
	
	//Panda::CanFrame frame;	// for building and sending
	PandaHealth health;
	
	int decimatorHeartbeat;
	int decimatorLka;
	int decimatorTrackB;
	int decimatorSteer;
	int decimatorAcc;
	
	unsigned char counterSteer;
	unsigned char counterTrackB;
	
	// HUD:
	bool hudLdaAlert;
	bool hudBarrier;//!barrier;
	unsigned char hudLeftLane;
	unsigned char hudRightLane;
	bool hudTwoBeeps;
	bool hudRepeatedBeeps;
	
	// Steering Control:
	double steerTorqueControl;
	bool steerRequest;
	unsigned char steerLkaState;
	
	// Acceleration Control:
	double accelerationControl;
	bool permitBraking;
	bool releaseStandstill;
	bool miniCar;
	bool cancelRequest;
	
	// Safety management, only send control commands if they are continuously updated
	int heartBeatSteer;
	int heartBeatAcceleration;
	
public:
	
	ToyotaHandler(Panda::Handler* handler);
	
	// Tells the driver to grab the steering wheel:
	void setHudLdaAlert( bool hudLdaAlert );
	// Shows barriers on the HUD
	void setHudBarrier( bool hudBarrier );
	// Shows divverent lanes for the right and left side.  Valid values 0-3:
	// 0: off
	// 1: white
	// 2: hollow white
	// 3: blinking orange
	void setHudLanes( unsigned char laneLeft, unsigned char laneRight );
	
	void setHudTwoBeeps( bool twoBeeps );
	void setHudRepeatedBeeps( bool repeatedBeeps );
	
	// Invokes a cruise contorl cancel request, for the driver to take control over vehicle
	void setHudCruiseCancelRequest( bool cancelRequest );
	
	/* The comma.ai panda code has the following limits for steeerTorque:
	const int TOYOTA_MAX_TORQUE = 1500;       // max torque cmd allowed ever
	 */
	void setSteerTorque( int steerTorque );
	
	/* The comma.ai panda code has the following limits for acceleration:
	const int TOYOTA_MAX_ACCEL = 1500;        // 1.5 m/s2
	const int TOYOTA_MIN_ACCEL = -3000;       // -3.0 m/s2
	 The following limits can be achieved by setting the panda into "unsafe" mode:
	const int TOYOTA_ISO_MAX_ACCEL = 2000;        // 2.0 m/s2
	const int TOYOTA_ISO_MIN_ACCEL = -3500;       // -3.5 m/s2
	
	 DBC range states: -20:20
	 */
	void setAcceleration( double acceleration );	// units are m/s2, valid range is -3.0:1.5
	
};

}

#endif
