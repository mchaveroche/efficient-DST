#ifndef MEMORY_POOL_HPP
#define MEMORY_POOL_HPP

#include <unordered_map>
#include <vector>
#include <iostream>


namespace ow_bft{

	class slot_indices {
	public:
		size_t block;
		size_t index;

		slot_indices(size_t _block, size_t _index) : block(_block), index(_index)
		{}
	};

	/*
	 * Stores contiguous elements with vectors, but split them in a vector to avoid reallocations (which invalidate pointers)
	 */
	template <class T>
	class memory_pool{
	protected:
		//const size_t tolerance = 10;
		size_t block_size;
		std::vector<std::vector<T> > slots;
		std::vector<std::vector<T*> > reusable_slots;
		std::unordered_map<T*, slot_indices> index_by_slot;
		//std::vector<std::vector<size_t> > free_indices;

	public:

		memory_pool(const size_t _block_size) : block_size(_block_size){
			add_new_block();
		}

		/////////////////////////////////////////

		const size_t& get_bock_size() const {
			return this->block_size;
		}

		template<typename... Ts>
		T* emplace(Ts... args){
			// if there is at least one slot returned to memory_pool

			// get the first contiguous block with a free reusable slot
			size_t b = 0;
			while(b < this->reusable_slots.size() && this->reusable_slots[b].size() == 0){
				++b;
			}

			if(b == this->slots.size()){
				// if there is none, create a new slot

				size_t b = this->slots.size()-1;
				if(this->slots.back().size() == this->block_size){
					// if the last block is full, create a new one
					add_new_block();
					++b;
				}
				size_t end_index = this->slots[b].size();
				// initialize slot with a new instance with parameters
				this->slots[b].emplace_back(args...);
				// create entry in index_by_slot with an object containing index and block index in this->slots
				// for this slot address in this block (for reverse querying)
				this->index_by_slot.emplace(&this->slots[b][end_index], slot_indices(b, end_index));
				// return a pointer to this free space in slots
				return &this->slots[b][end_index];

			}else{
				// otherwise, simply reuse the last one of them (LIFO)
				T* object_p = this->reusable_slots[b].back();
				// remove it from reusable slots
				this->reusable_slots[b].pop_back();
				*object_p = T(args...);
				// return a pointer to this free space in slots
				return object_p;
			}
		}

		void erase(T* slot){
			try {
				// get indices by querying index_by_slot with a slot address
				slot_indices sa = this->index_by_slot.at(slot);
				// if this address corresponds to the slot at the end of this->slots
				if(sa.block == this->slots.size()-1 && sa.index == this->slots[sa.block].size()-1){
					// erase that slot
					this->index_by_slot.erase(slot);
					this->slots.back().pop_back();

					// Also, if there is more than one block
					// AND the last block is entirely free
					// (AND the before last block has at least this->tolerance free_indices)
					if(this->slots.size() > 1 && this->slots.back().size() == 0
							//&& this->slots[this->slots.size()-2].size() <= this->block_size - this->tolerance
							){
						// then remove this last block
						this->slots.pop_back();
						this->reusable_slots.pop_back();
					}
				}else{
					// otherwise, reuse that slot
					this->reusable_slots[sa.block].emplace_back(slot);
				}
			}catch(const std::out_of_range& oor){
				// if this is not one of the address used in this memory pool
				std::cerr << "Out of Range error: " << oor.what() << '\n';
			}
		}

	protected:

		void add_new_block(){
			this->slots.emplace_back();
			this->slots.back().reserve(this->block_size);
			this->reusable_slots.emplace_back();
			this->reusable_slots.back().reserve(this->block_size);
		}
	};

}	// namespace ow_bft

#endif // MEMORY_POOL_HPP
