#pragma once

#include <stdint.h>

template <typename BufferType>
class Buffer
{
public:
	/// <summary>
	/// Create an Unitialized Buffer
	/// </summary>
	Buffer() {}

	/// <summary>
	/// Free Buffer Memory
	/// </summary>
	~Buffer() { if (m_Data) delete[] m_Data; }

	/// <summary>
	/// Create Buffer with a Specific Size
	/// </summary>
	/// <param name="Size"></param>
	Buffer(uint32_t Size) :m_Size(Size) { m_Data = new BufferType[Size]; }

	/// <summary>
	/// Create a Buffer with a Specific Size at a Specific Location
	/// </summary>
	/// <param name="Size"></param>
	/// <param name="Location"></param>
	Buffer(uint32_t Size, void* Location) :m_Size(Size) { m_Data = new(Location) BufferType[Size]; }

	/// <summary>
	/// Uses an Existing Buffer
	/// </summary>
	/// <param name="Size"></param>
	/// <param name="Data"></param>
	Buffer(uint32_t Size, BufferType* Data) :m_Size(Size), m_Data(Data) {}

	/// <summary>
	/// Create Buffer with a Specific Size
	/// </summary>
	/// <param name="Size"></param>
	void Create(uint32_t Size) { if (!m_Data) { m_Data = new BufferType[Size]; m_Size = Size; } }

	/// <summary>
	/// Create a Buffer with a Specific Size at a Specific Location
	/// </summary>
	/// <param name="Size"></param>
	void Create(uint32_t Size, void* Location) { if (!m_Data) { m_Data = new(Location) BufferType[Size]; m_Size = Size; } }
	
	/// <summary>
	/// Uses an Existing Buffer
	/// </summary>
	/// <param name="Size"></param>
	/// <param name="Data"></param>
	void Create(uint32_t Size, BufferType* Data) { if (!m_Data) { m_Data = Data; m_Size = Size; } }

	/// <summary>
	/// Gets Size of Buffer
	/// </summary>
	/// <returns>uint32_t</returns>
	inline uint32_t Size() const { return m_Size; }

	/// <summary>
	/// Returns Pointer to Buffer
	/// </summary>
	/// <returns></returns>
	inline BufferType* Get() { return m_Data; }

	/// <summary>
	/// {Operator} Returns Pointer to Buffer
	/// </summary>
	inline operator BufferType* () { return m_Data; }

	/// <summary>
	/// Returns a Const Pointer to Buffer
	/// </summary>
	/// <returns></returns>
	inline const BufferType* Get() const { return m_Data; }

	/// <summary>
	/// {Operator} Returns a Const Pointer to Buffer
	/// </summary>
	inline operator const BufferType* () const { return m_Data; }


	inline BufferType& operator[] (uint32_t index) { return m_Data[index]; }
	inline const BufferType& operator[] (uint32_t index) const { return m_Data[index]; }

private:
	BufferType* m_Data = nullptr;
	uint32_t m_Size = 0;
};
