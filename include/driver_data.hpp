//----------------------------------------------------------------------------//
//						- Final Project -
//----------------------------------------------------------------------------//
//
//	DESCRIPTION		Driver Data Class, Used to be delivered 
//					
//	AUTHOR 			Liad Raz
//	DATE			12 Apr 2022
//	VERSION     	v0.1
// 
//----------------------------------------------------------------------------//

#ifndef DRIVER_DATA_HPP
#define DRIVER_DATA_HPP

#include <vector>               // std::vector

// The Date Packet
struct DriverData
{
    enum RequestType {READ, WRITE};
    enum Status {SUCCESS, FAILED_TO_W, FAILED_TO_R, UNKNOWN_FAILURE};

	//
	// Special Members Constructor
    explicit DriverData() = default;
    ~DriverData() = default;
    DriverData(const DriverData& other_) = default;
    DriverData& operator=(const DriverData& other_) = default;
    
	//
	// Member Decelerations
    size_t m_handle;             
    RequestType m_type;
    size_t m_offset;
    size_t m_length;
    Status m_status;
    
    // NOTE     m_data will be allocated in the ctor (with the size of length_).
    std::vector<char> m_data; 
};



#endif // DRIVER_DATA_HPP
