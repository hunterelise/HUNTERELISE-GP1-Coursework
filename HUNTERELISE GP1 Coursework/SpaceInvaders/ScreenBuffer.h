#pragma once

struct ScreenBuffer {
public:
    ScreenBuffer() : row(0), col(0), m_buffer(nullptr) {}

    ScreenBuffer(int x, int y) : row(x), col(y), m_buffer(nullptr) {
        allocate();
    }

    ~ScreenBuffer() {
        deallocate();
    }

    // Move constructor
    ScreenBuffer(ScreenBuffer&& other) noexcept
        : row(other.row), col(other.col), m_buffer(other.m_buffer) {
        other.row = 0;
        other.col = 0;
        other.m_buffer = nullptr;
    }

    // Move assignment
    ScreenBuffer& operator=(ScreenBuffer&& other) noexcept {
        if (this != &other) {
            deallocate();
            row = other.row;
            col = other.col;
            m_buffer = other.m_buffer;
            other.row = 0;
            other.col = 0;
            other.m_buffer = nullptr;
        }
        return *this;
    }

    // Copy constructor
    ScreenBuffer(const ScreenBuffer& other)
        : row(other.row), col(other.col), m_buffer(nullptr) {
        allocate();
        for (int i = 0; i < row; ++i)
            for (int j = 0; j < col; ++j)
                m_buffer[i][j] = other.m_buffer[i][j];
    }

    // Copy assignment
    ScreenBuffer& operator=(const ScreenBuffer& other) {
        if (this == &other) return *this;

        deallocate();
        row = other.row;
        col = other.col;
        allocate();

        for (int i = 0; i < row; ++i)
            for (int j = 0; j < col; ++j)
                m_buffer[i][j] = other.m_buffer[i][j];

        return *this;
    }

    // Returns a character from the buffer
    char getChar(int r, int c) const {
        if (!m_buffer) return ' ';
        if (r < 0 || r >= row || c < 0 || c >= col) return ' ';
        return m_buffer[r][c];
    }

    // Sets a character in the buffer
    void setChar(int r, int c, char value) {
        if (!m_buffer) return;
        if (r < 0 || r >= row || c < 0 || c >= col) return;
        m_buffer[r][c] = value;
    }

private:
    // Allocates memory for the buffer
    void allocate() {
        if (row <= 0 || col <= 0) {
            m_buffer = nullptr;
            return;
        }

        m_buffer = new char* [row];
        for (int i = 0; i < row; ++i)
            m_buffer[i] = new char[col];
    }

    // Frees memory used by the buffer
    void deallocate() {
        if (!m_buffer) return;

        for (int i = 0; i < row; ++i)
            delete[] m_buffer[i];

        delete[] m_buffer;
        m_buffer = nullptr;
        row = col = 0;
    }

    int row;
    int col;
    char** m_buffer;
};
