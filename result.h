#pragma once

#include <utility>
#include <variant>

namespace result {

namespace detail {

/// Wrapper that encapsulates all logic shared between the error and success
/// types.
template <class T> class wrapper {
  static_assert(not std::is_reference<T>::value,
                "this wrapper stubbornly refuses to wrap references");

public:
  /// Converting copy constructor
  wrapper(T const &value) : value_(value) {}

  /// Converting move constructor
  wrapper(T &&value) : value_(std::move(value)) {}

  /// In-place construction
  template <class... Args>
  wrapper(Args... args) : value_(std::forward<Args>(args)...) {}

  T const &operator*() const { return value_; }
  T &operator*() { return value_; }

  T const &value() const { return value_; }
  T &value() { return value_; }

private:
  T value_;
};

// Specialization for void
template <> class wrapper<void> {};

} // namespace detail

// Inherit from wrapper instead of creating typedefs like
//   using success = detail::wrapper<T>;
//   using error = detail::wrapper<T>;
// since we want two different types for error and success, not two aliases for
// the same type.
template <class T> class success : public detail::wrapper<T> {
public:
  using detail::wrapper<T>::wrapper;
};

template <class T> class error : public detail::wrapper<T> {
public:
  using detail::wrapper<T>::wrapper;
};

template <class S, class E> class result {
public:
  // Typedefs
  using success_type = class success<S>;
  using error_type = class error<E>;

  // Functions
  result() = delete; // Do not allow "empty" results

  // Allow implicit conversions from success/error to result
  result(success_type s) : contents_(std::move(s)) {}
  result(error_type e) : contents_(std::move(e)) {}

  bool is_success() const noexcept {
    return std::holds_alternative<success_type>(contents_);
  }

  bool is_error() const noexcept {
    return std::holds_alternative<error_type>(contents_);
  }

  template<class T>
  using IsNotVoid = std::negation<std::is_void<T>>;

  template<class T=std::decay_t<S>,
    class sfinae=std::enable_if_t<IsNotVoid<T>::value, T>>
  T const &get() const { return std::get<success_type>(contents_).value(); }

  template<class T=std::decay_t<S>,
    class sfinae=std::enable_if_t<IsNotVoid<T>::value, T>>
  T &get() { return std::get<success_type>(contents_).value(); }

  template<class T=std::decay_t<S>,
    class sfinae=std::enable_if_t<IsNotVoid<T>::value, T>>
  T get_or(T alternative) const {
    if (is_success()) {
      return get();
    } else {
      return alternative;
    }
  }

  E const &error() const { return std::get<error_type>(contents_).value(); }
  E &error() { return std::get<error_type>(contents_).value(); }

private:
  // Data members
  std::variant<success_type, error_type> contents_;
};

} // namespace result
