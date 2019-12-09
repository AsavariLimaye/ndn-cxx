/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013-2018 Regents of the University of California.
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

#ifndef NDN_TRANSPORT_VMAC_TRANSPORT_HPP
#define NDN_TRANSPORT_VMAC_TRANSPORT_HPP

#include "ndn-cxx/transport/transport.hpp"
#include "ndn-cxx/util/config-file.hpp"
#include <boost/signals2.hpp>

namespace ndn {
class VmacTransport : public Transport
{
public:
  explicit
  VmacTransport();

  ~VmacTransport() override;

  void
  initVmac();

  void
  connect(boost::asio::io_service& ioService,
          const ReceiveCallback receiveCallback) override;

  void
  close() override;

  void
  pause() override;

  void
  resume() override;

  void
  send(const Block& wire) override;

  void
  send(const Block& wire, const std::string name) override;

  void
  send(const Block& header, const Block& payload) override;

  /** \brief Create transport with parameters defined in URI
   *  \throw Transport::Error incorrect URI or unsupported protocol is specified
   */
  static shared_ptr<VmacTransport>
  create(const std::string& uri);

  static boost::signals2::signal<void (uint8_t type,uint64_t enc, char* buff, uint16_t len, uint16_t seq, char* interestName, uint16_t interestNameLen)> m_signal;
private:
  static bool m_isRegistered;

  void vmacCallback(uint8_t type,uint64_t enc, char* buff, uint16_t len, uint16_t seq, char* interestName, uint16_t interestNameLen);
};

} // namespace ndn

#endif // NDN_TRANSPORT_VMAC_TRANSPORT_HPP
