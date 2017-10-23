#pragma once
//#include <boost\threadpool.hpp>
#include <boost\thread.hpp>
/**
* parallel_for_each algorithm
*/


template <class Itr, class Func>
void parallel_foreach_task(Itr first, Itr last, const Func& func)
{
	while (first != last)
		func(*first++);
}

//template <class Itr, class Func>
//void parallel_for_each(boost::threadpool::pool& pool, Itr first, Itr last, const Func& func)
//{
//	std::size_t range = std::distance(first, last);
//	std::size_t block = range / pool.size();
//	for (Itr next = first; first != last; first = next) {
//		std::advance(next, min(range, block));
//		range -= min(range, block);
//		if (next != last) {
//			pool.schedule(boost::bind(parallel_foreach_task<Itr, Func>, first, next, func));
//		} else {
//			parallel_foreach_task<Itr, Func>(first, next, func);
//		}
//	}
//	pool.wait();
//}
//
//template <class Range, class Func>
//void parallel_for_each(boost::threadpool::pool& pool, Range& range, const Func& func)
//{
//	parallel_for_each(pool, range.begin(), range.end(), func);
//}

template <class Itr, class Func>
void parallel_for_each(Itr first, Itr last, const Func& func, std::size_t num)
{
	boost::thread_group threadGroup;
	std::size_t range = std::distance(first, last);
	std::size_t block = range / num;
	for (Itr next = first; first != last; first = next) {
		std::advance(next, min(range, block));
		range -= min(range, block);
		if (next != last) {
			threadGroup.create_thread(boost::bind(parallel_foreach_task<Itr, Func>, first, next, func));
		} else {
			parallel_foreach_task<Itr, Func>(first, next, func);
		}
	}
	threadGroup.join_all();
}

template <class Range, class Func>
void parallel_for_each(Range& range, const Func& func, std::size_t num)
{
	parallel_for_each(range.begin(), range.end(), func, num);
}
