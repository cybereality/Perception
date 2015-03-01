#pragma once

namespace VRBoost
{
	/**
	* Float modification identifiers. 
	***/
	enum FloatModificationTypes
	{
		FloatDoNothing = 0,               /**< Simple modification that does not apply anything. **/
		FloatSimpleApply = 1,             /**< Simply applies the input float to the destination. **/
		FloatSimpleNegativeApply = 2,     /**< Simply applies the negative input float to the destination. **/
		FloatScale = 3,                   /**< Applies a scaled float. */
		FloatToDoubleScale = 4,           /**< Inputs a float and applies a scaled double. */
		FloatToBoolScale = 5,             /**< Inputs a float and applies a scaled bool. */
		FloatToByteScale = 6,             /**< Inputs a float and applies a scaled byte. */
		FloatToShortScale = 7,            /**< Inputs a float and applies a scaled short. */
		FloatToWordScale = 8,             /**< Inputs a float and applies a scaled WORD. */
		FloatToIntegerScale = 9,          /**< Inputs a float and applies a scaled int. */
		FloatToDWordScale = 10,           /**< Inputs a float and applies a scaled DWORD. */
		FloatToLongScale = 11,            /**< Inputs a float and applies a scaled long. */
		FloatToQWordScale = 12,           /**< Inputs a float and applies a scaled double. */
		FloatExtendedApply = 13,          /**< Simply applies the input float to the destination plus original value. **/
		FloatToSimpleDWord = 14,          /**< Inputs a float and casts to a DWORD and applies. */
		FloatDegreeApply = 80,            /**< Applies degrees from radian input. */
		FloatDegreeCompass = 81,          /**< Applies degrees from radian input and incorporates prior rotation angle. **/
		FloatDegreeNegativeApply = 82,    /**< Applies degrees from radian input. */
		FloatDegreeNegativeCompass = 83,  /**< Applies degrees from radian input and incorporates prior rotation angle. **/
		FloatDegreeStanley = 84,		  /**< Applies only to Stanley as it uses a smaller scale (-135 < x < 135). **/
		FloatDegreeStanleyPitch = 85,	  /**< Applies only to Stanley as it uses a smaller scale (-90 < x < 90). **/
		FloatDegreeChromeYaw = 86,		  /**< Applies only to Chrome Yaw Axis **/
		FloatGaussianCompass = 100,       /**< Applies the input float to a gaussian compass calculatian and incorporates prior rotation angle. **/
		FloatUnrealCompass = 101,         /**< Applies the input float to a UNREAL engine rotator compass calculatian and incorporates prior rotation angle. **/
		FloatUnrealCompass2 = 102,         /**< Applies the input float to a UNREAL engine rotator compass calculatian and incorporates prior rotation angle. **/
		FloatUnrealAxis = 201,            /**< Applies the input float to a UNREAL engine rotator axis. **/
		FloatUnrealNegativeAxis = 202,    /**< Applies the input float to a negative UNREAL engine rotator axis. **/
		FloatUnrealAxisWithOffsets = 203, /**< Applies the input float to a UNREAL engine rotator axis with offsets for horizon adjustment and pitch caps. **/
		FloatUnrealAxis2 = 204,            /**< Applies the input float to a UNREAL engine rotator axis. **/
		FloatCRYENGINEQuaternion = 300,	  /**< Applies input YPR to a Quaternion for CRYENGINE - Very specific to CryEngine games **/

		//The following are more general types for the memory scanners
		MemoryScanner = 300 			  /**< Will scan through a set of addresses until if finds candidate for yaw or FOV*/
	};

	/**
	* VRboost axis table.
	* Here, we set indices for the to-be-assigned VRboost input register.
	***/
	enum VRboostAxis
	{
		TrackerYaw = 0,                  /**< This index is reserved for Tracker->primaryYaw. **/
		TrackerPitch = 1,                /**< This index is reserved for Tracker->primaryPitch. **/
		TrackerRoll = 2,                 /**< This index is reserved for Tracker->primaryRoll. **/
		Zero = 3,                        /**< This index is reserved for constant 0 output. **/
		One = 4,                         /**< This index is reserved for constant 1 output. **/
		FreePitch = 5,					 /**< This index is reserved for Indicating Pitch axis shouldn't be locked (0.0 = FALSE, anything else = TRUE)**/
		// 6..11                              reserved for future tracker use
		// 12..23                             reserved for future input device use
		WorldFOV = 24,                   /**< Game-specific global FOV setting. **/
		PlayerFOV = 25,                  /**< Game-specific player surroundings FOV setting. **/
		FarPlaneFOV = 26,                /**< Game-specific FOV setting for the most distant objects. **/ 
		CameraTranslateX = 27,           /**< Translates the camera along the X-Axis. **/
		CameraTranslateY = 28,           /**< Translates the camera along the Y-Axis. **/
		CameraTranslateZ = 29,           /**< Translates the camera along the Z-Axis. **/
		CameraDistance = 30,             /**< Changes the distance of the camera. **/
		CameraZoom = 31,                 /**< Changes the cameras zoom. **/
		CameraHorizonAdjustment = 32,    /**< Adjusts the cameras X axis rotation to match the horizon. **/
		ConstantValue1 = 33,             /**< Game-specific constant value 1. **/
		ConstantValue2 = 34,             /**< Game-specific constant value 2. **/
		ConstantValue3 = 35,              /**< Game-specific constant value 3. **/
		NoAxis = 255						/**< No VRboost axis index **/
	};

	/**
	* Enum for the various ways we can check memory locations
	*/
	enum Comparison
	{
		NoCompare = 0,
		Equal,
		NotEqual,
		LessThan,
		GreaterThan,
		LessThanOrEqual,
		GreaterThanOrEqual,
		Between, //Between min max but not including
		BetweenIncl, //Pretty obvious isnt it?
		Outside //Not between min and max
	};


	/**
	* Custom defined return type from VRBoost
	*/
	enum ReturnValue
	{
		VRBOOST_CANDIDATES, /**< Scan completed but there were a number of candidate addresses */
		VRBOOST_OK, /**< Basic, "everything is ok" */
		VRBOOST_ERROR, /**< An error occured */
		VRBOOST_SCAN_READY, /**< VRBoost is ready to scan for memory locations */
		VRBOOST_SCAN_INITIALISING, /**< VRBoost is preparing scanner for memory locations */
		VRBOOST_SCANNING, /**< Memory scans are now in progress */
		VRBOOST_SCANNING_ASSIST, /**< Memory scans are now in progress, instructions need to be provided to the user (e.g. look up) */
		VRBOOST_SCAN_FAILED, /**< Memory scan failed completely to find a any appropriate addresses */
		VRBOOST_ADDRESSES_LOST /**< Memory scan addresses are no longer valid - rescan required*/
	};
}