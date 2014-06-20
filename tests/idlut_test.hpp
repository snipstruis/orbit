#pragma once

#include "utils/idlut.hpp"

#include <unittest++/UnitTest++.h>

SUITE(idlut_test){

	TEST(empty_container_has_size_0){
		OrderedIdlut<int,8> t;
		CHECK_EQUAL(0,t.size());
	}

	TEST(push_back_increases_size){
		OrderedIdlut<int,8> t;
		t.push_back(5);
		CHECK_EQUAL(1,t.size());
	}

	TEST(pushed_elemens_are_retrievable){
		OrderedIdlut<int,8> t;
		const auto
			is11 = t.push_back(11),
			is22 = t.push_back(22),
			is33 = t.push_back(33),
			is44 = t.push_back(44),
			is55 = t.push_back(55),
			is66 = t.push_back(66),
			is77 = t.push_back(77),
			is88 = t.push_back(88);
		CHECK_EQUAL(11,t[is11]);
		CHECK_EQUAL(22,t[is22]);
		CHECK_EQUAL(33,t[is33]);
		CHECK_EQUAL(44,t[is44]);
		CHECK_EQUAL(55,t[is55]);
		CHECK_EQUAL(66,t[is66]);
		CHECK_EQUAL(77,t[is77]);
		CHECK_EQUAL(88,t[is88]);
	}

	TEST(second_element_has_incremented_index){
		OrderedIdlut<int,8> t;
		auto id1 = t.push_back(111);
		auto id2 = t.push_back(222);
		CHECK_EQUAL(id1,id2-1);
	}

	TEST(pushing_in_new_container_is_contiguous_ascending){
		OrderedIdlut<int,8> t;
		const auto id1 = t.push_back(111);
		const auto id2 = t.push_back(222);
		const int* addr_first_element  = &t[id1];
		const int* addr_second_element = &t[id2];

		CHECK_EQUAL( addr_first_element+1,addr_second_element );
	}

	TEST(begin_points_to_first_element){
		OrderedIdlut<int,8> t;
		t.push_back(11);
		t.push_back(22);
		t.push_back(33);
		t.push_back(44);
		CHECK_EQUAL(11, *(t.begin()));
	}

	TEST(end_points_to_one_past_last_element){
		OrderedIdlut<int,8> t;
		t.push_back(11);
		t.push_back(22);
		t.push_back(33);
		t.push_back(44);
		CHECK_EQUAL(44, *(t.end()-1));
	}

	TEST(insert_before_works){
		OrderedIdlut<int,8> t;
		t.push_back(11);
		auto before_here = t.push_back(33);
		t.push_back(44);
		t.insert_before(before_here,22);
		CHECK_EQUAL(11,t.begin()[0]);
		CHECK_EQUAL(22,t.begin()[1]);
		CHECK_EQUAL(33,t.begin()[2]);
		CHECK_EQUAL(44,t.begin()[3]);
	}

	TEST(ordered_erase_preserves_order){
		OrderedIdlut<int,8> t;
		t.push_back(11);
		t.push_back(22);
		auto c = t.push_back(33);
		t.push_back(44);
		t.ordered_erase(c);
		CHECK_EQUAL(11, *t.begin());
		CHECK_EQUAL(22, *(t.begin()+1));
		CHECK_EQUAL(44, *(t.begin()+2));
	}

	TEST(ordered_erase_decreases_size){
		OrderedIdlut<int,8> t;
		t.push_back(11);
		auto c = t.push_back(22);
		t.ordered_erase(c);
		CHECK_EQUAL(1, t.size());
	}

	TEST(ordered_erase_multiple_elements){
		OrderedIdlut<int,8> t;
		t.push_back(11);
		auto a = t.push_back(22);
		t.push_back(33);
		t.push_back(44);
		t.ordered_erase(a,2);
		CHECK_EQUAL( 11, *t.begin());
		CHECK_EQUAL( 44, *(t.begin()+1));
		CHECK_EQUAL( 2,  t.size());
	}

}
