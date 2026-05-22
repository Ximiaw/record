#include <gtest/gtest.h>
#include "record.hpp"
#include <string>
#include <vector>
#include <memory>

// ============================================================================
// 默认构造函数测试
// ============================================================================

TEST(RecordTest, DefaultConstructEmptyRecord) {
    record<int,double> r;

    EXPECT_EQ(r.size(), 2);
}

TEST(RecordTest, DefaultConstructThenCopyEmptyRecord) {
    record<int,double> r1;
    record<int,double> r2(r1);

    EXPECT_EQ(r2.size(), 2);
}

TEST(RecordTest, DefaultConstructEmptyRecordMove) {
    record<int,double> r1;
    record<int,double> r2(std::move(r1));

    EXPECT_EQ(r2.size(), 2);
}

// ============================================================================
// 显式值构造测试
// ============================================================================

TEST(RecordTest, BasicConstructionAndGet) {
    record<int, double, std::string> r(42, 3.14, "hello");

    EXPECT_EQ(r.size(), 3);
    EXPECT_EQ(r.get<int>(0), 42);
    EXPECT_DOUBLE_EQ(r.get<double>(1), 3.14);
    EXPECT_EQ(r.get<std::string>(2), "hello");
}

TEST(RecordTest, SingleType) {
    record<int> r(100);

    EXPECT_EQ(r.size(), 1);
    EXPECT_EQ(r.get<int>(0), 100);
}

TEST(RecordTest, MultipleTypes) {
    record<int, char, float, long> r(1, 'a', 2.5f, 1000L);

    EXPECT_EQ(r.size(), 4);
    EXPECT_EQ(r.get<int>(0), 1);
    EXPECT_EQ(r.get<char>(1), 'a');
    EXPECT_FLOAT_EQ(r.get<float>(2), 2.5f);
    EXPECT_EQ(r.get<long>(3), 1000L);
}

TEST(RecordTest, MutableGet) {
    record<int, double> r(10, 1.5);

    r.get<int>(0) = 99;
    r.get<double>(1) = 2.718;

    EXPECT_EQ(r.get<int>(0), 99);
    EXPECT_DOUBLE_EQ(r.get<double>(1), 2.718);
}

// ============================================================================
// 重复类型测试
// ============================================================================

TEST(RecordTest, AllSameType) {
    record<int, int, int> r(10, 20, 30);

    EXPECT_EQ(r.size(), 3);
    EXPECT_EQ(r.get<int>(0), 10);
    EXPECT_EQ(r.get<int>(1), 20);
    EXPECT_EQ(r.get<int>(2), 30);
}

TEST(RecordTest, SameTypeAtDifferentPositions) {
    record<int, double, int> r(100, 3.14, 200);

    EXPECT_EQ(r.size(), 3);
    EXPECT_EQ(r.get<int>(0), 100);
    EXPECT_DOUBLE_EQ(r.get<double>(1), 3.14);
    EXPECT_EQ(r.get<int>(2), 200);
}

TEST(RecordTest, SameTypeInterleaved) {
    record<int, std::string, int, std::string, int> r(1, "a", 2, "b", 3);

    EXPECT_EQ(r.size(), 5);
    EXPECT_EQ(r.get<int>(0), 1);
    EXPECT_EQ(r.get<std::string>(1), "a");
    EXPECT_EQ(r.get<int>(2), 2);
    EXPECT_EQ(r.get<std::string>(3), "b");
    EXPECT_EQ(r.get<int>(4), 3);
}

TEST(RecordTest, DuplicateTypeWrongIndexThrows) {
    record<int, double, int> r(100, 3.14, 200);

    EXPECT_THROW(r.get<int>(1), std::runtime_error);
}

TEST(RecordTest, DuplicateTypeOutOfRangeThrows) {
    record<int, int, int> r(1, 2, 3);

    EXPECT_THROW(r.get<int>(3), std::out_of_range);
    EXPECT_THROW(r.get<int>(99), std::out_of_range);
}

TEST(RecordTest, DuplicateTypeMutableGet) {
    record<int, int, int> r(10, 20, 30);

    r.get<int>(0) = 100;
    r.get<int>(1) = 200;
    r.get<int>(2) = 300;

    EXPECT_EQ(r.get<int>(0), 100);
    EXPECT_EQ(r.get<int>(1), 200);
    EXPECT_EQ(r.get<int>(2), 300);
}

TEST(RecordTest, DuplicateComplexType) {
    record<std::vector<int>, std::vector<int>> r(
        std::vector<int>{1, 2},
        std::vector<int>{3, 4, 5}
    );

    EXPECT_EQ(r.size(), 2);
    EXPECT_EQ(r.get<std::vector<int>>(0).size(), 2);
    EXPECT_EQ(r.get<std::vector<int>>(1).size(), 3);
    EXPECT_EQ(r.get<std::vector<int>>(0)[0], 1);
    EXPECT_EQ(r.get<std::vector<int>>(1)[2], 5);
}

// ============================================================================
// 异常安全测试
// ============================================================================

TEST(RecordTest, OutOfRangeThrows) {
    record<int, double> r(1, 2.0);

    EXPECT_THROW(r.get<int>(2), std::out_of_range);
    EXPECT_THROW(r.get<int>(99), std::out_of_range);
    EXPECT_THROW(r.get<double>(2), std::out_of_range);
}

TEST(RecordTest, TypeMismatchThrows) {
    record<int, double, std::string> r(42, 3.14, "test");

    EXPECT_THROW(r.get<double>(0), std::runtime_error);
    EXPECT_THROW(r.get<int>(1), std::runtime_error);
    EXPECT_THROW(r.get<int>(2), std::runtime_error);
}

TEST(RecordTest, OutOfRangePriorityOverType) {
    record<int, double> r(1, 2.0);

    EXPECT_THROW(r.get<int>(5), std::out_of_range);
    EXPECT_THROW(r.get<double>(5), std::out_of_range);
}

// ============================================================================
// 拷贝语义测试
// ============================================================================

TEST(RecordTest, CopyConstruction) {
    record<int, std::string> r1(42, "original");
    record<int, std::string> r2(r1);

    EXPECT_EQ(r2.get<int>(0), 42);
    EXPECT_EQ(r2.get<std::string>(1), "original");

    r2.get<int>(0) = 99;
    EXPECT_EQ(r1.get<int>(0), 42);
    EXPECT_EQ(r2.get<int>(0), 99);
}

TEST(RecordTest, CopyAssignment) {
    record<int, std::string> r1(42, "original");
    record<int, std::string> r2(0, "temp");

    r2 = r1;

    EXPECT_EQ(r2.get<int>(0), 42);
    EXPECT_EQ(r2.get<std::string>(1), "original");

    r2.get<int>(0) = 77;
    EXPECT_EQ(r1.get<int>(0), 42);
}

TEST(RecordTest, SelfCopyAssignment) {
    record<int, std::string> r(42, "test");
    r = r;

    EXPECT_EQ(r.get<int>(0), 42);
    EXPECT_EQ(r.get<std::string>(1), "test");
}

TEST(RecordTest, DuplicateTypeCopyConstruction) {
    record<int, std::string, int> r1(42, "hello", 99);
    record<int, std::string, int> r2(r1);

    EXPECT_EQ(r2.get<int>(0), 42);
    EXPECT_EQ(r2.get<std::string>(1), "hello");
    EXPECT_EQ(r2.get<int>(2), 99);

    r2.get<int>(0) = 0;
    r2.get<int>(2) = 0;
    EXPECT_EQ(r1.get<int>(0), 42);
    EXPECT_EQ(r1.get<int>(2), 99);
}

TEST(RecordTest, DuplicateTypeCopyAssignment) {
    record<int, double, int> r1(10, 2.5, 20);
    record<int, double, int> r2(0, 0.0, 0);

    r2 = r1;

    EXPECT_EQ(r2.get<int>(0), 10);
    EXPECT_DOUBLE_EQ(r2.get<double>(1), 2.5);
    EXPECT_EQ(r2.get<int>(2), 20);
}

// ============================================================================
// 移动语义测试
// ============================================================================

TEST(RecordTest, MoveConstruction) {
    record<int, std::string> r1(42, "move_me");
    record<int, std::string> r2(std::move(r1));

    EXPECT_EQ(r2.get<int>(0), 42);
    EXPECT_EQ(r2.get<std::string>(1), "move_me");
}

TEST(RecordTest, MoveAssignment) {
    record<int, std::string> r1(42, "move_me");
    record<int, std::string> r2(0, "temp");

    r2 = std::move(r1);

    EXPECT_EQ(r2.get<int>(0), 42);
    EXPECT_EQ(r2.get<std::string>(1), "move_me");
}

TEST(RecordTest, SelfMoveAssignment) {
    record<int, std::string> r(42, "test");
    r = std::move(r);

    EXPECT_EQ(r.get<int>(0), 42);
    EXPECT_EQ(r.get<std::string>(1), "test");
}

TEST(RecordTest, DuplicateTypeMoveConstruction) {
    record<int, std::string, int> r1(1, "move", 2);
    record<int, std::string, int> r2(std::move(r1));

    EXPECT_EQ(r2.get<int>(0), 1);
    EXPECT_EQ(r2.get<std::string>(1), "move");
    EXPECT_EQ(r2.get<int>(2), 2);
}

TEST(RecordTest, DuplicateTypeMoveAssignment) {
    record<int, double, int> r1(100, 1.5, 200);
    record<int, double, int> r2(0, 0.0, 0);

    r2 = std::move(r1);

    EXPECT_EQ(r2.get<int>(0), 100);
    EXPECT_DOUBLE_EQ(r2.get<double>(1), 1.5);
    EXPECT_EQ(r2.get<int>(2), 200);
}

// ============================================================================
// 类型系统与边界测试
// ============================================================================

TEST(RecordTest, ConstGet) {
    record<int, double> r(42, 3.14);
    const auto& cr = r;

    EXPECT_EQ(cr.get<int>(0), 42);
    EXPECT_DOUBLE_EQ(cr.get<double>(1), 3.14);
}

TEST(RecordTest, DuplicateTypeConstGet) {
    record<int, double, int> r(42, 3.14, 99);
    const auto& cr = r;

    EXPECT_EQ(cr.get<int>(0), 42);
    EXPECT_DOUBLE_EQ(cr.get<double>(1), 3.14);
    EXPECT_EQ(cr.get<int>(2), 99);
}

TEST(RecordTest, ConvertibleConstruction) {
    record<int, double> r(static_cast<short>(5), 2.5f);

    EXPECT_EQ(r.get<int>(0), 5);
    EXPECT_DOUBLE_EQ(r.get<double>(1), 2.5);
}

TEST(RecordTest, ComplexTypes) {
    record<std::vector<int>, std::string> r(std::vector<int>{1, 2, 3}, "hello");

    EXPECT_EQ(r.size(), 2);
    const auto& vec = r.get<std::vector<int>>(0);
    EXPECT_EQ(vec.size(), 3);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 2);
    EXPECT_EQ(vec[2], 3);
    EXPECT_EQ(r.get<std::string>(1), "hello");
}

TEST(RecordTest, UniquePointerType) {
    auto ptr = std::make_unique<int>(123);
    record<std::unique_ptr<int>> r(std::move(ptr));

    EXPECT_EQ(r.size(), 1);
    EXPECT_NE(r.get<std::unique_ptr<int>>(0), nullptr);
    EXPECT_EQ(*r.get<std::unique_ptr<int>>(0), 123);
}

// ============================================================================
// 辅助类型别名测试
// ============================================================================

TEST(RecordTest, TypeAlias) {
    record<int, double, char>::type<0> v0 = 10;
    record<int, double, char>::type<1> v1 = 3.14;
    record<int, double, char>::type<2> v2 = 'x';

    EXPECT_EQ(v0, 10);
    EXPECT_DOUBLE_EQ(v1, 3.14);
    EXPECT_EQ(v2, 'x');
}

// ============================================================================
// 主函数
// ============================================================================

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}