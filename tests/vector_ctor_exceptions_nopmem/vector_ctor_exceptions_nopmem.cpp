// SPDX-License-Identifier: BSD-3-Clause
/* Copyright 2018-2020, Intel Corporation */

#include "list_wrapper.hpp"
#include "unittest.hpp"

#include <libpmemobj++/make_persistent.hpp>

namespace nvobj = pmem::obj;

using vector_type = container_t<int>;

struct root {
	nvobj::persistent_ptr<vector_type> pptr;
};

/**
 * Test pmem::obj::vector default constructor.
 *
 * Call default constructor for volatile instance of
 * pmem::obj::vector. Expect pmem::pool_error exception is thrown.
 */
void
test_default_ctor()
{
	bool exception_thrown = false;
	try {
		vector_type v = {};
		(void)v;
		UT_ASSERT(0);
	} catch (pmem::pool_error &) {
		exception_thrown = true;
	} catch (std::exception &e) {
		UT_FATALexc(e);
	}
	UT_ASSERT(exception_thrown);
}

/**
 * Test pmem::obj::vector range constructor.
 *
 * Call range constructor for volatile instance of
 * pmem::obj::vector. Expect pmem::pool_error exception is thrown.
 */
void
test_iter_iter_ctor()
{
	int a[] = {0, 1, 2, 3, 4, 5};

	bool exception_thrown = false;
	try {
		vector_type v(std::begin(a), std::end(a));
		(void)v;
		UT_ASSERT(0);
	} catch (pmem::pool_error &) {
		exception_thrown = true;
	} catch (std::exception &e) {
		UT_FATALexc(e);
	}
	UT_ASSERT(exception_thrown);
}

/**
 * Test pmem::obj::vector fill constructor with elements with
 * default values.
 *
 * Call fill constructor for volatile instance of
 * pmem::obj::vector. Expect pmem::pool_error exception is thrown.
 */
void
test_size_ctor()
{
	bool exception_thrown = false;
	try {
		vector_type v(100);
		(void)v;
		UT_ASSERT(0);
	} catch (pmem::pool_error &) {
		exception_thrown = true;
	} catch (std::exception &e) {
		UT_FATALexc(e);
	}
	UT_ASSERT(exception_thrown);
}

/**
 * Test pmem::obj::vector fill constructor with elements with
 * custom values.
 *
 * Call fill constructor for volatile instance of
 * pmem::obj::vector. Expect pmem::pool_error exception is thrown.
 */
void
test_size_value_ctor()
{
	bool exception_thrown = false;
	try {
		vector_type v(100, 5);
		(void)v;
		UT_ASSERT(0);
	} catch (pmem::pool_error &) {
		exception_thrown = true;
	} catch (std::exception &e) {
		UT_FATALexc(e);
	}
	UT_ASSERT(exception_thrown);
}

/**
 * Test pmem::obj::vector copy constructor
 *
 * Call copy constructor for volatile instance of
 * pmem::obj::vector. Expect pmem::pool_error exception is thrown.
 */
void
test_copy_ctor(nvobj::pool<struct root> &pop)
{
	auto r = pop.root();

	try {
		nvobj::transaction::run(pop, [&] {
			r->pptr = nvobj::make_persistent<vector_type>();
		});
	} catch (std::exception &e) {
		UT_FATALexc(e);
	}

	bool exception_thrown = false;
	try {
		vector_type v(*r->pptr);
		(void)v;
		UT_ASSERT(0);
	} catch (pmem::pool_error &) {
		exception_thrown = true;
	} catch (std::exception &e) {
		UT_FATALexc(e);
	}
	UT_ASSERT(exception_thrown);

	try {
		nvobj::transaction::run(pop, [&] {
			nvobj::delete_persistent<vector_type>(r->pptr);
		});
	} catch (std::exception &e) {
		UT_FATALexc(e);
	}
}

/**
 * Test pmem::obj::vector initializer list constructor
 *
 * Call initializer list constructor for volatile instance of
 * pmem::obj::vector. Expect pmem::pool_error exception is thrown.
 */
void
test_initializer_list_ctor()
{
	bool exception_thrown = false;
	try {
		vector_type v(std::initializer_list<int>{1, 2, 3, 4});
		(void)v;
		UT_ASSERT(0);
	} catch (pmem::pool_error &) {
		exception_thrown = true;
	} catch (std::exception &e) {
		UT_FATALexc(e);
	}
	UT_ASSERT(exception_thrown);
}

/**
 * Test pmem::obj::vector move constructor
 *
 * Call move constructor for volatile instance of
 * pmem::obj::vector. Expect pmem::pool_error exception is thrown.
 */
void
test_move_ctor(nvobj::pool<struct root> &pop)
{
	auto r = pop.root();

	try {
		nvobj::transaction::run(pop, [&] {
			r->pptr = nvobj::make_persistent<vector_type>();
		});
	} catch (std::exception &e) {
		UT_FATALexc(e);
	}

	bool exception_thrown = false;
	try {
		vector_type v(std::move(*r->pptr));
		(void)v;
		UT_ASSERT(0);
	} catch (pmem::pool_error &) {
		exception_thrown = true;
	} catch (std::exception &e) {
		UT_FATALexc(e);
	}
	UT_ASSERT(exception_thrown);

	try {
		nvobj::transaction::run(pop, [&] {
			nvobj::delete_persistent<vector_type>(r->pptr);
		});
	} catch (std::exception &e) {
		UT_FATALexc(e);
	}
}

static void
test(int argc, char *argv[])
{
	if (argc < 2) {
		UT_FATAL("usage: %s file-name", argv[0]);
	}

	auto path = argv[1];
	auto pop = nvobj::pool<root>::create(path, "VectorTest: ctor_nopmem",
					     PMEMOBJ_MIN_POOL * 2,
					     S_IWUSR | S_IRUSR);

	test_copy_ctor(pop);
	test_default_ctor();
	test_initializer_list_ctor();
	test_iter_iter_ctor();
	test_move_ctor(pop);
	test_size_ctor();
	test_size_value_ctor();

	pop.close();
}

int
main(int argc, char *argv[])
{
	return run_test([&] { test(argc, argv); });
}
