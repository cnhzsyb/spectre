// Distributed under the MIT License.
// See LICENSE.txt for details.
#pragma once

#include <algorithm>
#include <functional>
#include <iterator>

#include "Utilities/Gsl.hpp"

namespace cpp20 {
/*!
 * \ingroup UtilitiesGroup
 * Reimplementation of std::swap that is constexpr;
 * taken from the LLVM source at
 * https://github.com/llvm-mirror/libcxx/blob/master/include/type_traits
 */
template <class T>
constexpr void swap(T& a, T& b) {
  T c(std::move(a));
  a = std::move(b);
  b = std::move(c);
}

/*!
 * \ingroup UtilitiesGroup
 * Reimplementation of std::iter_swap that is constexpr;
 * taken from the LLVM source at
 * https://github.com/llvm-mirror/libcxx/blob/master/include/type_traits
 */
template <class ForwardIt1, class ForwardIt2>
constexpr void iter_swap(ForwardIt1 a, ForwardIt2 b) {
  swap(*a, *b);
}

namespace detail {
/*!
 * \ingroup UtilitiesGroup
 * Reimplementation of std::reverse that is constexpr, for bidirectional
 * iterators; taken from the LLVM source at
 * https://github.com/llvm-mirror/libcxx/blob/master/include/algorithm
 */
template <class BidirIt>
constexpr void reverse(BidirIt first, BidirIt last,
                       std::bidirectional_iterator_tag /* unused */) {
  while (first != last) {
    if (first == --last) {
      break;
    }
    cpp20::iter_swap(first, last);
    ++first;
  }
}

/*!
 * \ingroup UtilitiesGroup
 * Reimplementation of std::reverse that is constexpr, for random access
 * iterators; taken from the LLVM source at
 * https://github.com/llvm-mirror/libcxx/blob/master/include/algorithm
 */
template <class RandomAccessIterator>
constexpr void reverse(RandomAccessIterator first, RandomAccessIterator last,
                       std::random_access_iterator_tag /* unused */) {
  if (first != last) {
    for (; first < --last; ++first) {
      cpp20::iter_swap(first, last);
    }
  }
}
}  // namespace detail

/*!
 * \ingroup UtilitiesGroup
 * Reimplementation of std::reverse that is constexpr;
 * taken from the LLVM source at
 * https://github.com/llvm-mirror/libcxx/blob/master/include/algorithm
 */
template <class BidirectionalIterator>
constexpr void reverse(BidirectionalIterator first,
                       BidirectionalIterator last) {
  cpp20::detail::reverse(first, last,
                         typename std::iterator_traits<
                             BidirectionalIterator>::iterator_category());
}

/*!
 * \ingroup UtilitiesGroup
 * Reimplementation of std::next_permutation that is constexpr,
 * for a generic comparator; taken from the LLVM source at
 * https://github.com/llvm-mirror/libcxx/blob/master/include/algorithm
 */
template <class Compare, class BidirectionalIterator>
constexpr bool next_permutation(BidirectionalIterator first,
                                BidirectionalIterator last, Compare comp) {
  BidirectionalIterator i = last;
  if (first == last || first == --i) {
    return false;
  }
  while (true) {
    BidirectionalIterator ip1 = i;
    if (comp(*--i, *ip1)) {
      BidirectionalIterator j = last;
      while (!comp(*i, *--j)) {
      }
      cpp20::swap(*i, *j);
      cpp20::reverse(ip1, last);
      return true;
    }
    if (i == first) {
      cpp20::reverse(first, last);
      return false;
    }
  }
}

/*!
 * \ingroup UtilitiesGroup
 * Reimplementation of std::next_permutation that is constexpr,
 * with less as the comparator; taken from the LLVM source at
 * https://github.com/llvm-mirror/libcxx/blob/master/include/algorithm
 */
template <class BidirectionalIterator>
constexpr bool next_permutation(BidirectionalIterator first,
                                BidirectionalIterator last) {
  return cpp20::next_permutation(
      first, last,
      std::less<
          typename std::iterator_traits<BidirectionalIterator>::value_type>());
}

/*!
 * \ingroup UtilitiesGroup
 * Reimplementation of std::find that is constexpr
 */
template <class InputIt, class T>
constexpr InputIt find(InputIt first, InputIt last, const T& value) {
  for (; first != last; ++first) {
    if (*first == value) {
      return first;
    }
  }
  return last;
}

/*!
 * \ingroup UtilitiesGroup
 * Reimplementation of std::find_if that is constexpr
 */
template <class InputIt, class UnaryPredicate>
constexpr InputIt find_if(InputIt first, InputIt last, UnaryPredicate p) {
  for (; first != last; ++first) {
    if (p(*first)) {
      return first;
    }
  }
  return last;
}

/*!
 * \ingroup UtilitiesGroup
 * Reimplementation of std::find_if_not that is constexpr
 */
template <class InputIt, class UnaryPredicate>
constexpr InputIt find_if_not(InputIt first, InputIt last, UnaryPredicate q) {
  for (; first != last; ++first) {
    if (!q(*first)) {
      return first;
    }
  }
  return last;
}
}  // namespace cpp20

/*!
 * \ingroup UtilitiesGroup
 * \brief Utility functions wrapping STL algorithms and additional algorithms.
 */
namespace alg {
/// Convenience wrapper around std::all_of
template <class Container, class UnaryPredicate>
decltype(auto) all_of(const Container& c, UnaryPredicate&& unary_predicate) {
  using std::begin;
  using std::end;
  return std::all_of(begin(c), end(c),
                     std::forward<UnaryPredicate>(unary_predicate));
}

/// Convenience wrapper around std::any_of
template <class Container, class UnaryPredicate>
decltype(auto) any_of(const Container& c, UnaryPredicate&& unary_predicate) {
  using std::begin;
  using std::end;
  return std::any_of(begin(c), end(c),
                     std::forward<UnaryPredicate>(unary_predicate));
}

/// Convenience wrapper around std::none_of
template <class Container, class UnaryPredicate>
decltype(auto) none_of(const Container& c, UnaryPredicate&& unary_predicate) {
  using std::begin;
  using std::end;
  return std::none_of(begin(c), end(c),
                      std::forward<UnaryPredicate>(unary_predicate));
}

/// Convenience wrapper around std::count
template <class Container, class T>
decltype(auto) count(const Container& c, const T& value) {
  using std::begin;
  using std::end;
  return std::count(begin(c), end(c), value);
}

/// Convenience wrapper around std::count_if
template <class Container, class UnaryPredicate>
decltype(auto) count_if(const Container& c, UnaryPredicate&& unary_predicate) {
  using std::begin;
  using std::end;
  return std::count_if(begin(c), end(c),
                       std::forward<UnaryPredicate>(unary_predicate));
}

/// Convenience wrapper around constexpr reimplementation of std::find
template <class Container, class T>
constexpr decltype(auto) find(Container&& c, const T& value) {
  using std::begin;
  using std::end;
  return cpp20::find(begin(std::forward<Container>(c)),
                     end(std::forward<Container>(c)), value);
}

/// Convenience wrapper around constexpr reimplementation of std::find_if
template <class Container, class UnaryPredicate>
constexpr decltype(auto) find_if(Container&& c,
                                 UnaryPredicate&& unary_predicate) {
  using std::begin;
  using std::end;
  return cpp20::find_if(begin(std::forward<Container>(c)),
                        end(std::forward<Container>(c)),
                        std::forward<UnaryPredicate>(unary_predicate));
}

/// Convenience wrapper around constexpr reimplementation of std::find_if_not
template <class Container, class UnaryPredicate>
constexpr decltype(auto) find_if_not(Container&& c,
                                     UnaryPredicate&& unary_predicate) {
  using std::begin;
  using std::end;
  return cpp20::find_if_not(begin(std::forward<Container>(c)),
                            end(std::forward<Container>(c)),
                            std::forward<UnaryPredicate>(unary_predicate));
}

/// Convenience wrapper around constexpr reimplementation of std::find, returns
/// `true` if `value` is in `c`.
template <class Container, class T>
constexpr bool found(const Container& c, const T& value) {
  using std::begin;
  using std::end;
  return cpp20::find(begin(c), end(c), value) != end(c);
}

/// Convenience wrapper around constexpr reimplementation of std::find_if,
/// returns `true` if the result of `cpp20::find_if` is not equal to `end(c)`.
template <class Container, class UnaryPredicate>
constexpr bool found_if(const Container& c, UnaryPredicate&& unary_predicate) {
  using std::begin;
  using std::end;
  return cpp20::find_if(begin(c), end(c),
                        std::forward<UnaryPredicate>(unary_predicate)) !=
         end(c);
}

/// Convenience wrapper around constexpr reimplementation of std::find_if_not,
/// returns `true` if the result of `cpp20::find_if_not` is not equal to
/// `end(c)`.
template <class Container, class UnaryPredicate>
constexpr bool found_if_not(const Container& c,
                            UnaryPredicate&& unary_predicate) {
  using std::begin;
  using std::end;
  return cpp20::find_if_not(begin(c), end(c),
                            std::forward<UnaryPredicate>(unary_predicate)) !=
         end(c);
}

/// @{
/// Convenience wrapper around std::for_each, returns the result of
/// `std::for_each(begin(c), end(c), f)`.
template <class Container, class UnaryFunction>
decltype(auto) for_each(const Container& c, UnaryFunction&& f) {
  using std::begin;
  using std::end;
  return std::for_each(begin(c), end(c), std::forward<UnaryFunction>(f));
}

template <class Container, class UnaryFunction>
decltype(auto) for_each(Container& c, UnaryFunction&& f) {
  using std::begin;
  using std::end;
  return std::for_each(begin(c), end(c), std::forward<UnaryFunction>(f));
}
/// @}

/// Convenience wrapper around std::equal, assumes containers `lhs` has at least
/// as many elements as `rhs`.
template <class Container, class Container2>
decltype(auto) equal(const Container& lhs, const Container2& rhs) {
  using std::begin;
  using std::end;
  return std::equal(begin(lhs), end(lhs), begin(rhs));
}

/// Convenience wrapper around std::equal, assumes containers `lhs` has at least
/// as many elements as `rhs`.
template <class Container, class Container2, class BinaryPredicate>
decltype(auto) equal(const Container& lhs, const Container2& rhs,
                     BinaryPredicate&& p) {
  using std::begin;
  using std::end;
  return std::equal(begin(lhs), end(lhs), begin(rhs),
                    std::forward<BinaryPredicate>(p));
}

/// Convenience wrapper around std::max_element
template <class Container>
constexpr decltype(auto) max_element(const Container& c) {
  using std::begin;
  using std::end;
  return std::max_element(begin(c), end(c));
}

/// Convenience wrapper around std::max_element
template <class Container, class Compare>
constexpr decltype(auto) max_element(const Container& c, Compare&& comp) {
  using std::begin;
  using std::end;
  return std::max_element(begin(c), end(c), std::forward<Compare>(comp));
}

/// Convenience wrapper around std::min_element
template <class Container>
constexpr decltype(auto) min_element(const Container& c) {
  using std::begin;
  using std::end;
  return std::min_element(begin(c), end(c));
}

/// Convenience wrapper around std::min_element
template <class Container, class Compare>
constexpr decltype(auto) min_element(const Container& c, Compare&& comp) {
  using std::begin;
  using std::end;
  return std::min_element(begin(c), end(c), std::forward<Compare>(comp));
}

/// Convenience wrapper around std::remove
template <class Container, class T>
decltype(auto) remove(Container& c, const T& value) {
  using std::begin;
  using std::end;
  return std::remove(begin(c), end(c), value);
}

/// Convenience wrapper around std::remove_if
template <class Container, class UnaryPredicate>
decltype(auto) remove_if(Container& c, UnaryPredicate&& unary_predicate) {
  using std::begin;
  using std::end;
  return std::remove_if(begin(c), end(c),
                        std::forward<UnaryPredicate>(unary_predicate));
}

/// Convenience wrapper around std::sort
template <class Container>
decltype(auto) sort(Container& c) {
  using std::begin;
  using std::end;
  return std::sort(begin(c), end(c));
}

/// Convenience wrapper around std::sort
template <class Container, class Compare>
decltype(auto) sort(Container& c, Compare&& comp) {
  using std::begin;
  using std::end;
  return std::sort(begin(c), end(c), std::forward<Compare>(comp));
}

/// Convenience wrapper around std::transform
template <class Container, class OutputIt, class UnaryOp>
decltype(auto) transform(const Container& c, OutputIt output_first,
                         UnaryOp&& unary_op) {
  using std::begin;
  using std::end;
  return std::transform(begin(c), end(c), output_first,
                        std::forward<UnaryOp>(unary_op));
}

/// Convenience wrapper around std::transform
template <class Container1, class Container2, class OutputIt, class BinaryOp>
decltype(auto) transform(const Container1& c1, const Container2& c2,
                         OutputIt output_first, BinaryOp&& binary_op) {
  using std::begin;
  using std::end;
  return std::transform(begin(c1), end(c1), begin(c2), output_first,
                        std::forward<BinaryOp>(binary_op));
}
}  // namespace alg
