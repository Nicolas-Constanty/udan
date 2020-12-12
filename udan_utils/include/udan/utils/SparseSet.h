#pragma once
#include <typeindex>
#include <vector>

namespace udan::utils
{
	template<typename Entity>
	class SparseSet {
	protected:
		std::vector<Entity> m_sparse;

	public:
		explicit SparseSet(size_t capacity = 512)
		{
			m_sparse.reserve(capacity);
		}
	};


	template<typename Dataset>
	auto GetDataAtIndex(Dataset &dataset, size_t index)
	{
		return dataset.GetAtPosition(index);
	}
	
	template<typename Dataset>
	size_t GetDataSize(Dataset& dataset)
	{
		return dataset.GetSize();
	}

	template<typename ...Datasets>
	class DataSetView
	{	
		std::tuple<Datasets& ...> m_datasets;
		
	public:
		DataSetView(Datasets& ...datasets) : m_datasets({std::forward<Datasets>(datasets) ...})
		{}
		
		auto Get(size_t index)
		{
			return std::tuple_cat(GetDataAtIndex(std::get<Datasets &>(m_datasets), index)...);
		}

		size_t GetSize()
		{
			std::vector<size_t> sizes = { GetDataSize(std::get<Datasets&>(m_datasets)) ... };
			size_t result = UINT_MAX;
			for (auto s: sizes)
			{
				if (s < result)
					result = s;
			}
			return result;
		}
	};

	template<typename  DataSet>
	class DatasetIterator
	{
		using ValueType = typename DataSet::ValueType;
		using PointerType = ValueType*;
		using ReferenceType = ValueType&;

		PointerType m_Ptr;
		
	public:
		DatasetIterator(PointerType ptr) : m_Ptr(ptr)
		{}

		ReferenceType operator*()
		{
			return *m_Ptr;
		}

		PointerType operator->()
		{
			return m_Ptr;
		}

		std::tuple<ReferenceType> GetTupleReference()
		{
			return { *m_Ptr };
		}
		
		_NODISCARD std::tuple<ReferenceType> GetTupleReference(const size_t _Off) const noexcept {
			return { (*this)[_Off] };
		}

		bool operator==(const DatasetIterator& other) const
		{
			return m_Ptr == other.m_Ptr;
		}

		bool operator!=(const DatasetIterator& other) const
		{
			return !(*this==other);
		}

		DatasetIterator& operator++() noexcept
		{
			++m_Ptr;
			return *this;
		}

		DatasetIterator operator++(int) noexcept
		{
			DatasetIterator tmp = *this;
			++*this;
			return tmp;
		}

		DatasetIterator& operator--() noexcept {
			--m_Ptr;
			return *this;
		}

		DatasetIterator operator--(int) noexcept {
			DatasetIterator tmp = *this;
			--*this;
			return tmp;
		}

		DatasetIterator& operator+=(const size_t _Off) noexcept {
			m_Ptr += _Off;
			return *this;
		}

		_NODISCARD DatasetIterator operator+(const size_t _Off) const noexcept {
			DatasetIterator tmp = *this;
			return tmp += _Off;
		}

		DatasetIterator& operator-=(const size_t _Off) noexcept {
			return *this += -_Off;
		}


		_NODISCARD DatasetIterator operator-(const size_t _Off) const noexcept {
			DatasetIterator tmp = *this;
			return tmp -= _Off;
		}

		_NODISCARD ReferenceType operator[](const size_t _Off) const noexcept {
			return *(*this + _Off);
		}
	};

	template<typename Entity, typename ComponentType>
	class DataSet : public  SparseSet<Entity> {
		std::vector<ComponentType> m_dense;

		using ValueType = ComponentType;
		using Iterator = DatasetIterator<DataSet<Entity, ComponentType>>;

	public:
		explicit DataSet(size_t capacity = 512) : SparseSet<Entity>(capacity)
		{
			m_dense.reserve(capacity);
		}

		FORCEINLINE void PushBack(Entity id, ComponentType& component)
		{
			size_t pos = m_dense.size();
			m_dense.push_back(component);
			this->m_sparse[id] = pos;
		}

		template<typename ...Args>
		FORCEINLINE void EmplaceBack(Entity id, Args&& ...args)
		{
			size_t pos = m_dense.size();
			if constexpr (std::is_aggregate_v<ComponentType>) {
				m_dense.push_back(ComponentType{ std::forward<Args>(args)... });
			}
			else {
				m_dense.emplace_back(std::forward<Args>(args)...);
			}
			this->m_sparse[id] = pos;
		}

		ComponentType &GetComponent(Entity id)
		{
			return m_dense[this->m_sparse[id]];
		}

		FORCEINLINE std::vector<ComponentType> &GetData()
		{
			return m_dense;
		}

		std::tuple<ComponentType &> GetAtPosition(size_t index)
		{
			return { m_dense[index] };
		}

		size_t GetSize() const
		{
			return  m_dense.size();
		}
	};
}
