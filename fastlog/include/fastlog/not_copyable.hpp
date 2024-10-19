//
// Created by Steve Tchatchouang
//

#if !defined(TKS_NOT_COPYABLE)
#define TKS_NOT_COPYABLE

class notcopyable {
public:
  notcopyable(const notcopyable &) = delete;
  void operator=(const notcopyable &) = delete;

protected:
  notcopyable() = default;
  ~notcopyable() = default;
};

#endif // TKS_NOT_COPYABLE




