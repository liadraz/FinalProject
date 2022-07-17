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

#include <memory>               // std::shared_ptr
#include "driver_data.hpp"      // DriverData


// << Interface >>
class IDriverComm
{

public:
	//
	// Special Members Constructor
    explicit IDriverComm() = default;
    virtual ~IDriverComm() = 0;

    // Cctor. copy= are blocked from use
    IDriverComm(const IDriverComm& other_) = delete;   
    IDriverComm& operator=(const IDriverComm& other_) = delete;
    
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
IDriverComm::~IDriverComm() = default;



#endif // IDRIVER_COMM_HPP