//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Copyright 2019-2020, Intel Corporation
//
// Modified to test pmem::obj containers
//

#include "unittest.hpp"

#include <libpmemobj++/container/string.hpp>

namespace nvobj = pmem::obj;

using S = pmem::obj::string;

struct root {
	nvobj::persistent_ptr<S> s;
};

template <class S>
void
test(S &s)
{
	try {
		while (s.size() < s.capacity())
			s.push_back(typename S::value_type());
		UT_ASSERT(s.size() == s.capacity());
	} catch (...) {
		UT_ASSERT(false);
	}
}

static void
test(int argc, char *argv[])
{
	if (argc < 2) {
		UT_FATAL("usage: %s file-name", argv[0]);
	}

	auto path = argv[1];
	auto pop = nvobj::pool<root>::create(
		path, "string_test", PMEMOBJ_MIN_POOL, S_IWUSR | S_IRUSR);

	auto r = pop.root();
	{
		try {
			nvobj::transaction::run(pop, [&] {
				r->s = nvobj::make_persistent<S>();
			});

			auto &s = *r->s;

			test(s);

			s.assign(10, 'a');
			s.erase(5);
			test(s);

			s.assign(100, 'a');
			s.erase(50);
			test(s);

			nvobj::transaction::run(pop, [&] {
				nvobj::delete_persistent<S>(r->s);
			});
		} catch (std::exception &e) {
			UT_FATALexc(e);
		}
	}

	pop.close();
}

int
main(int argc, char *argv[])
{
	return run_test([&] { test(argc, argv); });
}
