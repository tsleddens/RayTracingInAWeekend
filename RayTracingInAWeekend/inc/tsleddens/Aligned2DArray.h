#pragma once

namespace tsleddens
{
template<typename T, std::size_t Alignment = 32>
class Aligned2DArray
{
    static_assert( std::is_trivial_v<T> && std::is_standard_layout_v<T>, "Aligned2DArray requires a POD-like type" );

    UINT m_rows, m_cols;
    T**  m_ppRows;
    T*   m_pData;

public:
    Aligned2DArray( UINT cols, UINT rows )
    : m_rows( rows )
    , m_cols( cols )
    , m_ppRows( nullptr )
    , m_pData( nullptr )
    {
        Allocate( rows, cols );
    }

    Aligned2DArray( const Aligned2DArray& )            = delete;  // Disable copy constructor
    Aligned2DArray& operator=( const Aligned2DArray& ) = delete;  // Disable copy assignment

    Aligned2DArray( Aligned2DArray&& other ) noexcept
    : m_rows( other.m_rows )
    , m_cols( other.m_cols )
    , m_ppRows( other.m_ppRows )
    , m_pData( other.m_pData )
    {
        other.m_rows   = 0;
        other.m_cols   = 0;
        other.m_ppRows = nullptr;
        other.m_pData  = nullptr;
    }

    Aligned2DArray& operator=( Aligned2DArray&& other ) noexcept
    {
        if ( this != &other )
        {
            FreeMemory();
            m_rows         = other.m_rows;
            m_cols         = other.m_cols;
            m_ppRows       = other.m_ppRows;
            m_pData        = other.m_pData;
            other.m_rows   = 0;
            other.m_cols   = 0;
            other.m_ppRows = nullptr;
            other.m_pData  = nullptr;
        }
        return *this;
    }

    ~Aligned2DArray()
    {
        FreeMemory();
    }

    T* Raw()
    {
        return m_pData;
    }

    const T* operator&() const
    {
        return m_pData;
    }

    [[nodiscard]] const T* Raw() const
    {
        return m_pData;
    }

    void Resize( UINT cols, UINT rows )
    {
        if ( rows == m_rows && cols == m_cols )
        {
            return;
        }

        FreeMemory();
        Allocate( rows, cols );
    }

    void CopyInto( void* ppDestination ) const
    {
        if ( !ppDestination )
        {
            throw std::invalid_argument( "Destination pointer cannot be null." );
        }
        std::memcpy( ppDestination, m_pData, m_rows * m_cols * sizeof( T ) );
    }

    inline T* operator[]( int i )
    {
        return m_ppRows[i];
    }

    inline const T* operator[]( int i ) const
    {
        return m_ppRows[i];
    }

private:
    void Allocate( UINT rows, UINT cols )
    {
        m_rows = rows;
        m_cols = cols;

        m_pData = static_cast<T*>( _aligned_malloc( rows * cols * sizeof( T ), Alignment ) );
        if ( !m_pData )
        {
            throw std::bad_alloc();
        }

        m_ppRows = static_cast<T**>( std::malloc( rows * sizeof( T* ) ) );
        if ( !m_ppRows )
        {
            std::free( m_pData );
            m_pData = nullptr;
            throw std::bad_alloc();
        }

        for ( UINT i = 0; i < rows; ++i )
        {
            m_ppRows[i] = m_pData + i * cols;
        }

        std::memset( m_pData, 0, rows * cols * sizeof( T ) );
    }

    void FreeMemory()
    {
        std::free( m_ppRows );
        _aligned_free( m_pData );
        m_pData  = nullptr;
        m_ppRows = nullptr;
    }
};
}  // namespace tsleddens