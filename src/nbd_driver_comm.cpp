//----------------------------------------------------------------------------//
//						- Final Project -
//----------------------------------------------------------------------------//
//
//	DESCRIPTION		Implementation  nbd_driver device
//					
//	AUTHOR 			Liad Raz
//	DATE			12 Apr 2022
//	VERSION     	v0.1
// 
//----------------------------------------------------------------------------//

#include <sys/socket.h>         // socketpair
#include <cassert>              // assert
#include <fcntl.h>              // open
#include <sys/ioctl.h>          // ioctl
#include <linux/nbd.h>          // NBD_SET_BLKSIZE

#include "nbd_driver_comm.hpp"     // 


//----------------------------------------------------------------------------//
//	NBDDriverComm Definitions
//----------------------------------------------------------------------------//
// Create sockets and init NBDDriverComm class members
NBDDriverComm::NBDDriverComm
    (const std::string& device_path_, size_t storage_size_)
{
    InitDriverSocketsFD();
    ConnectNBDDevice(device_path_, storage_size_);
}


NBDDriverComm::NBDDriverComm
    (const std::string& device_path_, size_t num_blocks_, size_t block_size_)
{
    InitDriverSocketsFD();
    ConnectNBDDevice(device_path_, CalcStorageSizeIMP(num_blocks_, block_size_));
}

//
// Utils Functions
//----------------------------------------------------------------------------//
void NBDDriverComm::InitDriverSocketsFD()
{
    int socketPairFD[2];
    int error = 0;

    // Creates a pair of sockets which are connected to each other;
    // In a related processes; Each socket is a biodirectional.
    error = socketpair(AF_UNIX, SOCK_STREAM, 0, socketPairFD);
    assert(!error);

    m_sockFds[DriverSoc] = socketPairFD[0];      // Server - Driver Master App
    m_sockFds[NBDSoc] = socketPairFD[1];         // Client - NBD device

}

void NBDDriverComm::ConnectNBDDevice(const std::string& device_path_, size_t storage_size_)
{
    // TODO: Write  Exception for each syscall

    // get fd for the NBD actual device
    m_nbdFd = open(&device_path_, O_RDWR);

    // Set nbd Storage size
    ioctl(m_nbdFd, NBD_SET_BLKSIZE, storage_size_);
    
    // Reset the setup_task. Incase an app exits before running BD_DO_IT but after adding sockets we can
    // end up not being allowed to do a new nbd device.
    ioctl(m_nbdFd, NBD_CLEAR_SOCK);

    // Connect NBD device to the NBD socket descriptor
    ioctl(m_nbdFd, NBD_SET_SOCK, m_sockFds[NBDSoc]);
}


size_t NBDDriverComm::CalcStorageSizeIMP(size_t num_blocks_, size_t block_size_)
{
    return num_blocks_ * block_size_;
}


NBDDriverComm::~NBDDriverComm()
{}


// Member functions - Main Functionality
//----------------------------------------------------------------------------//
std::shared_ptr<DriverData> NBDDriverComm::ReceiveRequest()
{

}

void NBDDriverComm::SendReply(std::shared_ptr<const DriverData> data_)
{

}

void NBDDriverComm::Disconnect()
{

}


int NBDDriverComm::GetFD() const
{
    return m_sockFds[NBDSoc];
}
