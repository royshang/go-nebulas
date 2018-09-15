#include "common/util/byte.h"
#include <gtest/gtest.h>

TEST(test_common_util, simple) {
  int32_t v = 123;
  neb::byte_t buf[4];
  neb::util::number_to_byte(v, buf, 4);

  int32_t ret = neb::util::byte_to_number<int32_t>(buf, 4);

  EXPECT_EQ(v, ret);
}

template <typename NT>
void test_number_bytes(NT v, const std::initializer_list<neb::byte_t> &l) {
  neb::util::bytes b = neb::util::number_to_byte<neb::util::bytes>(v);

  neb::util::bytes wb(l);
  EXPECT_EQ(b, wb);
  EXPECT_EQ(v, neb::util::byte_to_number<NT>(wb));
}
TEST(test_common_util_byte, from_uint64) {
  test_number_bytes<uint64_t>(0, {0, 0, 0, 0, 0, 0, 0, 0});
  test_number_bytes<uint64_t>(1024, {0, 0, 0, 0, 0, 0, 4, 0});
  test_number_bytes<uint64_t>(18446744073709551615u,
                              {255, 255, 255, 255, 255, 255, 255, 255});
}

TEST(test_common_util_byte, from_uint32) {
  test_number_bytes<uint32_t>(0, {0, 0, 0, 0});
  test_number_bytes<uint32_t>(1024, {0, 0, 4, 0});
  test_number_bytes<uint32_t>(4294967295u, {255, 255, 255, 255});
}

TEST(test_common_util_byte, from_uint16) {
  test_number_bytes<uint16_t>(0, {0, 0});
  test_number_bytes<uint16_t>(1024, {4, 0});
  test_number_bytes<uint16_t>(65535u, {255, 255});
}

TEST(test_common_util, from_int64) {
  test_number_bytes<int64_t>(0, {0, 0, 0, 0, 0, 0, 0, 0});
  test_number_bytes<int64_t>(1024, {0, 0, 0, 0, 0, 0, 4, 0});
  test_number_bytes<int64_t>(-1024, {255, 255, 255, 255, 255, 255, 252, 0});
  test_number_bytes<int64_t>(9223372036854775807,
                             {127, 255, 255, 255, 255, 255, 255, 255});
  test_number_bytes<int64_t>(-9223372036854775808ll,
                             {128, 0, 0, 0, 0, 0, 0, 0});
}

TEST(test_common_util, from_int32) {
  test_number_bytes<int32_t>(0, {0, 0, 0, 0});
  test_number_bytes<int32_t>(1024, {0, 0, 4, 0});
  test_number_bytes<int32_t>(-1024, {255, 255, 252, 0});
  test_number_bytes<int32_t>(2147483647, {127, 255, 255, 255});
  test_number_bytes<int32_t>(-2147483648, {128, 0, 0, 0});
}
TEST(test_common_util, from_int16) {
  test_number_bytes<int16_t>(0, {0, 0});
  test_number_bytes<int16_t>(1024, {4, 0});
  test_number_bytes<int16_t>(-1024, {252, 0});
  test_number_bytes<int16_t>(32767, {127, 255});
  test_number_bytes<int16_t>(-32768, {128, 0});
}

TEST(test_common_util_byte, positive_to_string) {
  neb::util::bytes bytes(
      {72, 101, 108, 108, 111, 44, 32, 119, 111, 114, 108, 100});
  std::string result = neb::util::byte_to_string(bytes);
  std::string want("Hello, world");
  EXPECT_EQ(result, want);

  neb::util::bytes bs = neb::util::string_to_byte(result);
  EXPECT_EQ(bs, bytes);
}


TEST(test_common_util_byte, test_default) {
  neb::util::fix_bytes<> fb;

  std::string base58 = fb.to_base58();

  EXPECT_EQ(base58, "11111111111111111111111111111111");
}

TEST(test_common_util_byte, positive_fix_bytes_to_base58) {
  neb::util::fix_bytes<6> fb({32, 119, 111, 114, 108, 100});

  std::string result = fb.to_base58();
  std::cout << result << std::endl;
  neb::util::fix_bytes<6> tb = neb::util::fix_bytes<6>::from_base58(result);

  EXPECT_EQ(fb, tb);
}

TEST(test_common_util_byte, positive_fix_bytes_to_hex) {
  neb::util::fix_bytes<6> fb({132, 11, 111, 104, 18, 100});

  std::string result = fb.to_hex();
  neb::util::fix_bytes<6> tb = neb::util::fix_bytes<6>::from_hex(result);
  EXPECT_EQ(fb, tb);

  auto tf = [](const std::string &hexstring,
               const std::initializer_list<neb::byte_t> &hexbytes) {
    neb::util::bytes bytes(hexbytes);
    EXPECT_EQ(bytes.to_hex(), hexstring);
    EXPECT_EQ(bytes, neb::util::bytes::from_hex(hexstring));
  };
  tf("a7ffc6f8bf1ed76651c14756a061d662f580ff4de43b49fa82d80a4b80f8434a",
     {167, 255, 198, 248, 191, 30,  215, 102, 81, 193, 71,
      86,  160, 97,  214, 98,  245, 128, 255, 77, 228, 59,
      73,  250, 130, 216, 10,  75,  128, 248, 67, 74});
  tf("3550aba97492de38af3066f0157fc532db6791b37d53262ce7688dcc5d461856",
     {53,  80, 171, 169, 116, 146, 222, 56,  175, 48,  102,
      240, 21, 127, 197, 50,  219, 103, 145, 179, 125, 83,
      38,  44, 231, 104, 141, 204, 93,  70,  24,  86});

  tf("", {});
}

