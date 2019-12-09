/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013-2019 Regents of the University of California.
 *
 * This file is part of ndn-cxx library (NDN C++ library with eXperimental eXtensions).
 *
 * ndn-cxx library is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later version.
 *
 * ndn-cxx library is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.
 *
 * You should have received copies of the GNU General Public License and GNU Lesser
 * General Public License along with ndn-cxx, e.g., in COPYING.md file.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * See AUTHORS.md for complete list of ndn-cxx authors and contributors.
 */

#include "ndn-cxx/transport/vmac-transport.hpp"

#include "ndn-cxx/face.hpp"
#include "ndn-cxx/net/face-uri.hpp"
#include "ndn-cxx/util/logger.hpp"
#include <vmac/vmac.h>

NDN_LOG_INIT(ndn.VmacTransport);
// DEBUG level: connect, close, pause, resume.

namespace ndn {

void vmac_callback(uint8_t type,uint64_t enc, char* buff, uint16_t len, uint16_t seq, char* interestName, uint16_t interestNameLen)
{
  NDN_LOG_DEBUG("Type: " << type << "  Name: " << interestName << "  Data Len: " << len);
  VmacTransport::m_signal(type, enc, buff, len, seq, interestName, interestNameLen);
}

boost::signals2::signal<void (uint8_t type,uint64_t enc, char* buff, uint16_t len, uint16_t seq, char* interestName, uint16_t interestNameLen)> VmacTransport::m_signal;
bool VmacTransport::m_isRegistered = false;

VmacTransport::VmacTransport()
{
  initVmac();  
}

VmacTransport::~VmacTransport() = default;

void
VmacTransport::initVmac()
{
  if (VmacTransport::m_isRegistered)
	            return;
  VmacTransport::m_isRegistered = true;
  void (*ptr) (uint8_t a, uint64_t b, char* c, uint16_t d, uint16_t e, char* f, uint16_t g) = &vmac_callback;	
  VmacTransport::m_signal.connect(boost::bind(&VmacTransport::vmacCallback, this, _1, _2, _3, _4, _5, _6, _7));
  vmac_register((void*) ptr);
  NDN_LOG_DEBUG("Vmac Interface Initialized");
}

shared_ptr<VmacTransport>
VmacTransport::create(const std::string& uri)
{
  //return make_shared<VmacTransport>(getSocketNameFromUri(uri));
  return make_shared<VmacTransport>();
}

void
VmacTransport::connect(boost::asio::io_service& ioService,
                       const ReceiveCallback receiveCallback)
{
  NDN_LOG_DEBUG("Connect VMAC");
}

void
VmacTransport::send(const Block& wire)
{
}

void
VmacTransport::send(const Block& wire, const std::string name)
{

  ndn::EncodingBuffer enc_buffer(wire);
  size_t buff_len = enc_buffer.size();
  size_t interest_len = name.length();

  char buffptr[buff_len + 1];
  char interest_name[interest_len + 1];

  strncpy(buffptr, (char*) enc_buffer.buf(), buff_len);
  strncpy(interest_name, name.c_str(), interest_len);

  if (buff_len < 2000){
    NDN_LOG_DEBUG("send : Sending Interest " << name);
    send_vmac(0, 0, 0, (char*) enc_buffer.buf(), (uint16_t) buff_len, (char*) interest_name, (uint16_t) interest_len);
  }
}

void
VmacTransport::send(const Block& header, const Block& payload)
{
}

void
VmacTransport::close()
{
}

void
VmacTransport::pause()
{
}

void
VmacTransport::resume()
{
}

void
VmacTransport::vmacCallback(uint8_t type,uint64_t enc, char* buff, uint16_t len, uint16_t seq, char* interestName, uint16_t interestNameLen)
{
  bool isOk = false;
  Block element;
  std::tie(isOk, element) = Block::fromBuffer((uint8_t*) buff, len);
  if (!isOk)
    NDN_LOG_DEBUG("Received invalid block");
  else
    this->m_receiveCallback(element);
}

} // namespace ndn
