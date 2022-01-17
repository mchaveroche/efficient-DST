#ifndef EFFICIENT_DST_PIGNISTIC_PROBABILITY_HPP
#define EFFICIENT_DST_PIGNISTIC_PROBABILITY_HPP

#include <mass.hpp>

namespace efficient_DST{

	template <typename T, size_t N>
	class pignistic_probability {
	protected:
		const mass<T, N> source_mass;

		T compute_aggregation(const std::bitset<N>& setA, const bool& singleton) const {
			T sum = 0;

			if (singleton){
				const std::vector<set_N_value<T, N>* >& supersets = this->source_mass.get_definition().supersets_of(setA);

				for (size_t i = 0; i < supersets.size(); ++i) {
					sum += supersets[i]->value / supersets[i]->cardinality;
				}
			}else{
				const std::map<size_t, std::vector<set_N_value<T, N>* >, std::less<size_t> >& cardinality_map = this->source_mass.get_definition().elements_by_ascending_cardinality();

				for (auto kv : cardinality_map) {
					if (kv.first != 0) {
						for (size_t i = 0; i < kv.second.size(); ++i) {
							const set_N_value<T, N>* B = kv.second[i];
							const std::bitset<N>& intersection = setA & B->set;

							sum += B->value * intersection.count() / kv.first;
						}
					}
				}
			}
			return sum;
		}

	public:

		pignistic_probability(const mass<T, N>& m) : source_mass(m)
		{}

		pignistic_probability(const pignistic_probability<T, N>& bet_p) : source_mass(bet_p.source_mass)
		{}

		std::vector<T> get_contour() {
			std::vector<T> contour;
			contour.reserve(N);
			std::bitset<N> singleton = 0;

			for(size_t i = 0; i < N; ++i){
				singleton[i] = true;
				contour.emplace_back(compute_aggregation(singleton, true));
				singleton[i] = false;
			}
			return contour;
		}

		T at_emptyset() const {
			return 0;
		}

		T at_fod() const {
			std::bitset<N> fod = 0;
			fod.set();
			return compute_aggregation(fod, false);
		}

		T operator[](const std::vector<std::string>& labels) const {
			return find(this->source_mass.get_FOD().to_set(labels));
		}

		T find(const std::bitset<N>& set) const {
			bool is_singleton = false;
			if (set.count() == 1){
				is_singleton = true;
			}
			return compute_aggregation(set, is_singleton);
		}
	};

} // namespace efficient_DST

#endif // EFFICIENT_DST_PIGNISTIC_PROBABILITY_HPP
