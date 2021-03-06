#include <gtest/gtest.h>
#include <zasm/zasm.hpp>

namespace zasm::tests
{
    TEST(SerializationTests, LeaRax0X64)
    {
        using namespace zasm::operands;

        Program program(ZYDIS_MACHINE_MODE_LONG_64);
        Assembler assembler(program);
        Serializer serializer;

        ASSERT_EQ(assembler.lea(rax, qword_ptr(0)), Error::None);
        ASSERT_EQ(serializer.serialize(program, 0x0000000000401000), Error::None);

        const std::array<uint8_t, 8> expected = {
            0x48, 0x8D, 0x04, 0x25, 0x00, 0x00, 0x00, 0x00,
        };
        ASSERT_EQ(serializer.getCodeSize(), expected.size());

        const auto* data = serializer.getCode();
        ASSERT_NE(data, nullptr);
        for (size_t i = 0; i < expected.size(); i++)
        {
            ASSERT_EQ(data[i], expected[i]);
        }
    }

    TEST(SerializationTests, LeaEax0X86)
    {
        using namespace zasm::operands;

        Program program(ZYDIS_MACHINE_MODE_LONG_COMPAT_32);
        Assembler assembler(program);
        Serializer serializer;

        ASSERT_EQ(assembler.lea(eax, dword_ptr(0)), Error::None);
        ASSERT_EQ(serializer.serialize(program, 0x0000000000401000), Error::None);

        const std::array<uint8_t, 6> expected = {
            0x8D, 0x05, 0x00, 0x00, 0x00, 0x00,
        };
        ASSERT_EQ(serializer.getCodeSize(), expected.size());

        const auto* data = serializer.getCode();
        ASSERT_NE(data, nullptr);
        for (size_t i = 0; i < expected.size(); i++)
        {
            ASSERT_EQ(data[i], expected[i]);
        }
    }

    TEST(SerializationTests, LeaRipLabelX64)
    {
        using namespace zasm::operands;

        Program program(ZYDIS_MACHINE_MODE_LONG_64);
        Assembler assembler(program);
        Serializer serializer;

        auto label = assembler.createLabel();

        ASSERT_EQ(assembler.lea(rax, qword_ptr(rip, label)), Error::None);
        ASSERT_EQ(assembler.bind(label), Error::None);
        ASSERT_EQ(assembler.dq(0x123C312F21329F31ULL), Error::None);
        ASSERT_EQ(serializer.serialize(program, 0x0000000000401000), Error::None);

        const std::array<uint8_t, 15> expected = {
            0x48, 0x8D, 0x05, 0x00, 0x00, 0x00, 0x00, 0x31, 0x9F, 0x32, 0x21, 0x2F, 0x31, 0x3C, 0x12,
        };
        ASSERT_EQ(serializer.getCodeSize(), expected.size());

        const auto* data = serializer.getCode();
        ASSERT_NE(data, nullptr);
        for (size_t i = 0; i < expected.size(); i++)
        {
            ASSERT_EQ(data[i], expected[i]);
        }
    }

    TEST(SerializationTests, LeaLabelX86)
    {
        using namespace zasm::operands;

        Program program(ZYDIS_MACHINE_MODE_LONG_COMPAT_32);
        Assembler assembler(program);
        Serializer serializer;

        auto label = assembler.createLabel();

        ASSERT_EQ(assembler.lea(eax, dword_ptr(label)), Error::None);
        ASSERT_EQ(assembler.bind(label), Error::None);
        ASSERT_EQ(assembler.dq(0x123C312F21329F31ULL), Error::None);
        ASSERT_EQ(serializer.serialize(program, 0x0000000000401000), Error::None);

        const std::array<uint8_t, 14> expected = { 0x8D, 0x05, 0x06, 0x10, 0x40, 0x00, 0x31,
                                                   0x9F, 0x32, 0x21, 0x2F, 0x31, 0x3C, 0x12 };
        ASSERT_EQ(serializer.getCodeSize(), expected.size());

        const auto* data = serializer.getCode();
        ASSERT_NE(data, nullptr);
        for (size_t i = 0; i < expected.size(); i++)
        {
            ASSERT_EQ(data[i], expected[i]);
        }
    }

    TEST(SerializationTests, JmpLabelRel8FrontX64)
    {
        using namespace zasm::operands;

        Program program(ZYDIS_MACHINE_MODE_LONG_64);
        Assembler assembler(program);
        Serializer serializer;

        auto label = assembler.createLabel();

        ASSERT_EQ(assembler.jmp(label), Error::None);
        for (int i = 0; i < 124; i++)
            ASSERT_EQ(assembler.nop(), Error::None);
        ASSERT_EQ(assembler.bind(label), Error::None);
        ASSERT_EQ(assembler.int3(), Error::None);
        ASSERT_EQ(serializer.serialize(program, 0x0000000000401000), Error::None);

        const std::array<uint8_t, 127> expected = {
            0xEB, 0x7C, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0xCC,
        };
        ASSERT_EQ(serializer.getCodeSize(), expected.size());

        const auto* data = serializer.getCode();
        ASSERT_NE(data, nullptr);
        for (size_t i = 0; i < expected.size(); i++)
        {
            ASSERT_EQ(data[i], expected[i]);
        }
    }

    TEST(SerializationTests, JmpLabelRel8FrontX86)
    {
        using namespace zasm::operands;

        Program program(ZYDIS_MACHINE_MODE_LONG_COMPAT_32);
        Assembler assembler(program);
        Serializer serializer;

        auto label = assembler.createLabel();

        ASSERT_EQ(assembler.jmp(label), Error::None);
        for (int i = 0; i < 124; i++)
            ASSERT_EQ(assembler.nop(), Error::None);
        ASSERT_EQ(assembler.bind(label), Error::None);
        ASSERT_EQ(assembler.int3(), Error::None);
        ASSERT_EQ(serializer.serialize(program, 0x0000000000401000), Error::None);

        const std::array<uint8_t, 127> expected = {
            0xEB, 0x7C, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0xCC,
        };

        ASSERT_EQ(serializer.getCodeSize(), expected.size());

        const auto* data = serializer.getCode();
        ASSERT_NE(data, nullptr);
        for (size_t i = 0; i < expected.size(); i++)
        {
            ASSERT_EQ(data[i], expected[i]);
        }
    }

    TEST(SerializationTests, JmpLabelRel8BackX64)
    {
        using namespace zasm::operands;

        Program program(ZYDIS_MACHINE_MODE_LONG_64);
        Assembler assembler(program);
        Serializer serializer;

        auto label = assembler.createLabel();

        ASSERT_EQ(assembler.bind(label), Error::None);
        ASSERT_EQ(assembler.int3(), Error::None);
        for (int i = 0; i < 124; i++)
            ASSERT_EQ(assembler.nop(), Error::None);
        ASSERT_EQ(assembler.jmp(label), Error::None);
        ASSERT_EQ(serializer.serialize(program, 0x0000000000401000), Error::None);

        const std::array<uint8_t, 127> expected = {
            0xCC, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0xEB, 0x81,
        };
        ASSERT_EQ(serializer.getCodeSize(), expected.size());

        const auto* data = serializer.getCode();
        ASSERT_NE(data, nullptr);
        for (size_t i = 0; i < expected.size(); i++)
        {
            ASSERT_EQ(data[i], expected[i]);
        }
    }

    TEST(SerializationTests, JmpLabelRel8BackX86)
    {
        using namespace zasm::operands;

        Program program(ZYDIS_MACHINE_MODE_LONG_COMPAT_32);
        Assembler assembler(program);
        Serializer serializer;

        auto label = assembler.createLabel();

        ASSERT_EQ(assembler.bind(label), Error::None);
        ASSERT_EQ(assembler.int3(), Error::None);
        for (int i = 0; i < 124; i++)
            ASSERT_EQ(assembler.nop(), Error::None);
        ASSERT_EQ(assembler.jmp(label), Error::None);
        ASSERT_EQ(serializer.serialize(program, 0x0000000000401000), Error::None);

        const std::array<uint8_t, 127> expected = {
            0xCC, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0xEB, 0x81,
        };
        ASSERT_EQ(serializer.getCodeSize(), expected.size());

        const auto* data = serializer.getCode();
        ASSERT_NE(data, nullptr);
        for (size_t i = 0; i < expected.size(); i++)
        {
            ASSERT_EQ(data[i], expected[i]);
        }
    }

    TEST(SerializationTests, JmpLabelRel32BackX64)
    {
        using namespace zasm::operands;

        Program program(ZYDIS_MACHINE_MODE_LONG_64);
        Assembler assembler(program);
        Serializer serializer;

        auto label = assembler.createLabel();

        ASSERT_EQ(assembler.bind(label), Error::None);
        ASSERT_EQ(assembler.int3(), Error::None);
        for (int i = 0; i < 125; i++)
            ASSERT_EQ(assembler.nop(), Error::None);
        ASSERT_EQ(assembler.jmp(label), Error::None);
        ASSERT_EQ(serializer.serialize(program, 0x0000000000401000), Error::None);

        const std::array<uint8_t, 131> expected = {
            0xCC, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0xE9, 0x7D, 0xFF, 0xFF, 0xFF,
        };
        ASSERT_EQ(serializer.getCodeSize(), expected.size());

        const auto* data = serializer.getCode();
        ASSERT_NE(data, nullptr);
        for (size_t i = 0; i < expected.size(); i++)
        {
            ASSERT_EQ(data[i], expected[i]);
        }
    }

    TEST(SerializationTests, JmpLabelRel32BackX86)
    {
        using namespace zasm::operands;

        Program program(ZYDIS_MACHINE_MODE_LONG_COMPAT_32);
        Assembler assembler(program);
        Serializer serializer;

        auto label = assembler.createLabel();

        ASSERT_EQ(assembler.bind(label), Error::None);
        ASSERT_EQ(assembler.int3(), Error::None);
        for (int i = 0; i < 125; i++)
            ASSERT_EQ(assembler.nop(), Error::None);
        ASSERT_EQ(assembler.jmp(label), Error::None);
        ASSERT_EQ(serializer.serialize(program, 0x0000000000401000), Error::None);

        const std::array<uint8_t, 131> expected = {
            0xCC, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0xE9, 0x7D, 0xFF, 0xFF, 0xFF,
        };
        ASSERT_EQ(serializer.getCodeSize(), expected.size());

        const auto* data = serializer.getCode();
        ASSERT_NE(data, nullptr);
        for (size_t i = 0; i < expected.size(); i++)
        {
            ASSERT_EQ(data[i], expected[i]);
        }
    }

    TEST(SerializationTests, UnboundLabel)
    {
        using namespace zasm::operands;

        Program program(ZYDIS_MACHINE_MODE_LONG_64);
        Assembler assembler(program);
        Serializer serializer;

        auto label01 = assembler.createLabel();

        ASSERT_EQ(assembler.nop(), Error::None);
        ASSERT_EQ(assembler.lea(rax, qword_ptr(label01)), Error::None);
        ASSERT_EQ(assembler.nop(), Error::None);

        ASSERT_EQ(serializer.serialize(program, 0x0000000000401000), Error::UnresolvedLabel);
    }

    TEST(SerializationTests, EmbeddedLabelX64)
    {
        using namespace zasm::operands;

        Program program(ZYDIS_MACHINE_MODE_LONG_64);
        Assembler assembler(program);
        Serializer serializer;

        auto label01 = assembler.createLabel();

        ASSERT_EQ(assembler.bind(label01), Error::None);
        ASSERT_EQ(assembler.nop(), Error::None);
        ASSERT_EQ(assembler.nop(), Error::None);
        ASSERT_EQ(assembler.embedLabel(label01), Error::None);

        ASSERT_EQ(serializer.serialize(program, 0x0000000000401000), Error::None);

        const std::array<uint8_t, 10> expected = { 0x90, 0x90, 0x00, 0x10, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00 };
        ASSERT_EQ(serializer.getCodeSize(), expected.size());

        const auto* data = serializer.getCode();
        ASSERT_NE(data, nullptr);
        for (size_t i = 0; i < expected.size(); i++)
        {
            ASSERT_EQ(data[i], expected[i]);
        }
    }

    TEST(SerializationTests, EmbeddedLabelX86)
    {
        using namespace zasm::operands;

        Program program(ZYDIS_MACHINE_MODE_LEGACY_32);
        Assembler assembler(program);
        Serializer serializer;

        auto label01 = assembler.createLabel();

        ASSERT_EQ(assembler.bind(label01), Error::None);
        ASSERT_EQ(assembler.nop(), Error::None);
        ASSERT_EQ(assembler.nop(), Error::None);
        ASSERT_EQ(assembler.embedLabel(label01), Error::None);

        ASSERT_EQ(serializer.serialize(program, 0x0000000000401000), Error::None);

        const std::array<uint8_t, 6> expected = { 0x90, 0x90, 0x00, 0x10, 0x40, 0x00 };
        ASSERT_EQ(serializer.getCodeSize(), expected.size());

        const auto* data = serializer.getCode();
        ASSERT_NE(data, nullptr);
        for (size_t i = 0; i < expected.size(); i++)
        {
            ASSERT_EQ(data[i], expected[i]);
        }
    }

    TEST(SerializationTests, EmbeddedLabelRel8X64)
    {
        using namespace zasm::operands;

        Program program(ZYDIS_MACHINE_MODE_LONG_64);
        Assembler assembler(program);
        Serializer serializer;

        auto label01 = assembler.createLabel();
        auto label02 = assembler.createLabel();

        ASSERT_EQ(assembler.bind(label01), Error::None);
        ASSERT_EQ(assembler.nop(), Error::None);
        ASSERT_EQ(assembler.bind(label02), Error::None);
        ASSERT_EQ(assembler.nop(), Error::None);
        ASSERT_EQ(assembler.embedLabelRel(label02, label01, BitSize::_8), Error::None);

        ASSERT_EQ(serializer.serialize(program, 0x0000000000401000), Error::None);

        const std::array<uint8_t, 3> expected = { 0x90, 0x90, 0x01 };
        ASSERT_EQ(serializer.getCodeSize(), expected.size());

        const auto* data = serializer.getCode();
        ASSERT_NE(data, nullptr);
        for (size_t i = 0; i < expected.size(); i++)
        {
            ASSERT_EQ(data[i], expected[i]);
        }
    }

    TEST(SerializationTests, EmbeddedLabelRel8BeforeEmbedX64)
    {
        using namespace zasm::operands;

        Program program(ZYDIS_MACHINE_MODE_LONG_64);
        Assembler assembler(program);
        Serializer serializer;

        auto label01 = assembler.createLabel();
        auto label02 = assembler.createLabel();

        ASSERT_EQ(assembler.bind(label01), Error::None);
        ASSERT_EQ(assembler.embedLabelRel(label02, label01, BitSize::_8), Error::None);
        ASSERT_EQ(assembler.nop(), Error::None);
        ASSERT_EQ(assembler.bind(label02), Error::None);
        ASSERT_EQ(assembler.nop(), Error::None);

        ASSERT_EQ(serializer.serialize(program, 0x0000000000401000), Error::None);

        const std::array<uint8_t, 3> expected = { 0x02, 0x90, 0x90 };
        ASSERT_EQ(serializer.getCodeSize(), expected.size());

        const auto* data = serializer.getCode();
        ASSERT_NE(data, nullptr);
        for (size_t i = 0; i < expected.size(); i++)
        {
            ASSERT_EQ(data[i], expected[i]);
        }
    }

    TEST(SerializationTests, EmbeddedLabelRel8BeforeEmbedNegativeX64)
    {
        using namespace zasm::operands;

        Program program(ZYDIS_MACHINE_MODE_LONG_64);
        Assembler assembler(program);
        Serializer serializer;

        auto label01 = assembler.createLabel();
        auto label02 = assembler.createLabel();

        ASSERT_EQ(assembler.bind(label01), Error::None);
        ASSERT_EQ(assembler.embedLabelRel(label01, label02, BitSize::_8), Error::None);
        ASSERT_EQ(assembler.nop(), Error::None);
        ASSERT_EQ(assembler.bind(label02), Error::None);
        ASSERT_EQ(assembler.nop(), Error::None);

        ASSERT_EQ(serializer.serialize(program, 0x0000000000401000), Error::None);

        const std::array<uint8_t, 3> expected = { 0xFE, 0x90, 0x90 };
        ASSERT_EQ(serializer.getCodeSize(), expected.size());

        const auto* data = serializer.getCode();
        ASSERT_NE(data, nullptr);
        for (size_t i = 0; i < expected.size(); i++)
        {
            ASSERT_EQ(data[i], expected[i]);
        }
    }

    TEST(SerializationTests, EmbeddedLabelRel16BeforeEmbedX64)
    {
        using namespace zasm::operands;

        Program program(ZYDIS_MACHINE_MODE_LONG_64);
        Assembler assembler(program);
        Serializer serializer;

        auto label01 = assembler.createLabel();
        auto label02 = assembler.createLabel();

        ASSERT_EQ(assembler.bind(label01), Error::None);
        ASSERT_EQ(assembler.embedLabelRel(label02, label01, BitSize::_16), Error::None);
        ASSERT_EQ(assembler.nop(), Error::None);
        ASSERT_EQ(assembler.bind(label02), Error::None);
        ASSERT_EQ(assembler.nop(), Error::None);

        ASSERT_EQ(serializer.serialize(program, 0x0000000000401000), Error::None);

        const std::array<uint8_t, 4> expected = { 0x03, 0x00, 0x90, 0x90 };
        ASSERT_EQ(serializer.getCodeSize(), expected.size());

        const auto* data = serializer.getCode();
        ASSERT_NE(data, nullptr);
        for (size_t i = 0; i < expected.size(); i++)
        {
            ASSERT_EQ(data[i], expected[i]);
        }
    }

    TEST(SerializationTests, EmbeddedLabelRel16BeforeEmbedNegativeX64)
    {
        using namespace zasm::operands;

        Program program(ZYDIS_MACHINE_MODE_LONG_64);
        Assembler assembler(program);
        Serializer serializer;

        auto label01 = assembler.createLabel();
        auto label02 = assembler.createLabel();

        ASSERT_EQ(assembler.bind(label01), Error::None);
        ASSERT_EQ(assembler.embedLabelRel(label01, label02, BitSize::_16), Error::None);
        ASSERT_EQ(assembler.nop(), Error::None);
        ASSERT_EQ(assembler.bind(label02), Error::None);
        ASSERT_EQ(assembler.nop(), Error::None);

        ASSERT_EQ(serializer.serialize(program, 0x0000000000401000), Error::None);

        const std::array<uint8_t, 4> expected = { 0xFD, 0xFF, 0x90, 0x90 };
        ASSERT_EQ(serializer.getCodeSize(), expected.size());

        const auto* data = serializer.getCode();
        ASSERT_NE(data, nullptr);
        for (size_t i = 0; i < expected.size(); i++)
        {
            ASSERT_EQ(data[i], expected[i]);
        }
    }

    TEST(SerializationTests, EmbeddedLabelRel32BeforeEmbedX64)
    {
        using namespace zasm::operands;

        Program program(ZYDIS_MACHINE_MODE_LONG_64);
        Assembler assembler(program);
        Serializer serializer;

        auto label01 = assembler.createLabel();
        auto label02 = assembler.createLabel();

        ASSERT_EQ(assembler.bind(label01), Error::None);
        ASSERT_EQ(assembler.embedLabelRel(label02, label01, BitSize::_32), Error::None);
        ASSERT_EQ(assembler.nop(), Error::None);
        ASSERT_EQ(assembler.bind(label02), Error::None);
        ASSERT_EQ(assembler.nop(), Error::None);

        ASSERT_EQ(serializer.serialize(program, 0x0000000000401000), Error::None);

        const std::array<uint8_t, 6> expected = { 0x05, 0x00, 0x00, 0x00, 0x90, 0x90 };
        ASSERT_EQ(serializer.getCodeSize(), expected.size());

        const auto* data = serializer.getCode();
        ASSERT_NE(data, nullptr);
        for (size_t i = 0; i < expected.size(); i++)
        {
            ASSERT_EQ(data[i], expected[i]);
        }
    }

    TEST(SerializationTests, EmbeddedLabelRel32BeforeEmbedNegativeX64)
    {
        using namespace zasm::operands;

        Program program(ZYDIS_MACHINE_MODE_LONG_64);
        Assembler assembler(program);
        Serializer serializer;

        auto label01 = assembler.createLabel();
        auto label02 = assembler.createLabel();

        ASSERT_EQ(assembler.bind(label01), Error::None);
        ASSERT_EQ(assembler.embedLabelRel(label01, label02, BitSize::_32), Error::None);
        ASSERT_EQ(assembler.nop(), Error::None);
        ASSERT_EQ(assembler.bind(label02), Error::None);
        ASSERT_EQ(assembler.nop(), Error::None);

        ASSERT_EQ(serializer.serialize(program, 0x0000000000401000), Error::None);

        const std::array<uint8_t, 6> expected = { 0xFB, 0xFF, 0xFF, 0xFF, 0x90, 0x90 };
        ASSERT_EQ(serializer.getCodeSize(), expected.size());

        const auto* data = serializer.getCode();
        ASSERT_NE(data, nullptr);
        for (size_t i = 0; i < expected.size(); i++)
        {
            ASSERT_EQ(data[i], expected[i]);
        }
    }

} // namespace zasm::tests