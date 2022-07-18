//----------------------------------------------------------------------------//
//						- Final Project -
//----------------------------------------------------------------------------//
//
//	DESCRIPTION		INTERFACE class for Device
//					
//	AUTHOR 			Liad Raz
//	DATE			12 Apr 2022
//	VERSION     	v0.1
// 
//----------------------------------------------------------------------------//


#ifndef IDRIVER_COMM_HPP
#define IDRIVER_COMM_HPP

#include <memory>           // std::shared_ptr
#include <string>           // std::string
#include <stdexcept>        // std::runtime_error

#include "driver_data.hpp"      // DriverData


// << Interface >>
class IDriver
{

public:
	//
	// Special Members Constructor
    explicit IDriver() = default;
    virtual ~IDriver() = 0;

    // Cctor. copy= are blocked from use
    IDriver(const IDriver& other_) = delete;   
    IDriver& operator=(const IDriver& other_) = delete;
    
	//
	// Main Functionality
    virtual std::shared_ptr<DriverData> ReceiveRequest() = 0;
    virtual void SendReply(std::shared_ptr<const DriverData> data_) = 0;
    virtual void Disconnect() = 0;

	//
	// Extra Functionality
    virtual int GetFD() const = 0;
};

// Impl of the virtual Dtor
IDriver::~IDriver() = default;

//
// Exception Error Handling
struct IDriverError: public std::runtime_error
{
    IDriverError(const std::string& str_ = "IDrive error");
};


#endif // IDRIVER_COMM_HPP