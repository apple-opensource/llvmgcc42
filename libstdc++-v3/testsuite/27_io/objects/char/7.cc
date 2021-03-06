// { dg-require-fork "" }
// { dg-require-mkfifo "" }

// 2003-04-26 Petur Runolfsson  <peturr02@ru.is>

// Copyright (C) 2003, 2004, 2005, 2006 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 2, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING.  If not, write to the Free
// Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
// USA.

// 27.3 Standard iostream objects

#include <fstream>
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

// No asserts, avoid leaking the semaphore if a VERIFY fails.
#undef _GLIBCXX_ASSERT

#include <testsuite_hooks.h>

// Check that cout.flush() is called when last ios_base::Init is destroyed.
bool test07()
{
  using namespace std;
  using namespace __gnu_test;
  bool test __attribute__((unused)) = true;

  const char* name = "tmp_fifo4";

  signal(SIGPIPE, SIG_IGN);

  unlink(name);  
  mkfifo(name, S_IRWXU);
  semaphore s1;

  int child = fork();
  VERIFY( child != -1 );

  if (child == 0)
    {
      filebuf fbout;
      fbout.open(name, ios_base::in|ios_base::out);
      VERIFY( fbout.is_open() );
      s1.wait();
      cout.rdbuf(&fbout);
      fbout.sputc('a');
      // NB: fbout is *not* destroyed here!
      exit(0);
    }
  
  filebuf fbin;
  fbin.open(name, ios_base::in);
  s1.signal();
  filebuf::int_type c = fbin.sbumpc();
  VERIFY( c != filebuf::traits_type::eof() );
  VERIFY( c == filebuf::traits_type::to_int_type('a') );

  fbin.close();

  return test;
}

int
main()
{
  return !test07();
}
