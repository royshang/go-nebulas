// Copyright (C) 2018 go-nebulas authors
//
// This file is part of the go-nebulas library.
//
// the go-nebulas library is free software: you can redistribute it and/or
// modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// the go-nebulas library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with the go-nebulas library.  If not, see
// <http://www.gnu.org/licenses/>.
//
#include "common/exception_queue.h"

namespace neb {
void exception_queue::push_back(const std::exception_ptr &p) {
  std::unique_lock<std::mutex> _l(m_mutex);
  bool was_empty = m_exceptions.empty();
  m_exceptions.push_back(p);
  _l.unlock();
  if (was_empty) {
    m_cond_var.notify_one();
  }
}

std::exception_ptr exception_queue::pop_front() {
  std::unique_lock<std::mutex> _l(m_mutex);

  if (m_exceptions.empty()) {
    m_cond_var.wait(_l);
  }
  if (m_exceptions.empty()) {
    return nullptr;
  }
  std::exception_ptr ret = m_exceptions.front();
  m_exceptions.erase(m_exceptions.begin());
  return ret;
}

} // namespace neb
