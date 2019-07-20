#ifndef OW_BFT_IMPLICABILITY_HPP
#define OW_BFT_IMPLICABILITY_HPP

#include <mass_aggregate.hpp>

namespace ow_bft{

	template <typename T = double>
	class implicability : public mass_aggregate<T> {
	protected:

		T compute_aggregation_at_emptyset() const {
			return compute_aggregation_at_emptyset(this->mass_equivalent.get_focal_elements());
		}

		T compute_aggregation_at_fod() const {
			return compute_aggregation_at_fod(this->mass_equivalent.get_focal_elements());
		}

		T compute_aggregation(const boost::dynamic_bitset<>& set) const {
			return compute_aggregation(this->mass_equivalent.get_focal_elements(), set);
		}

	public:

		implicability(const mass<T>& m) : mass_aggregate<T>(m)
		{
			compute_values_for_mass_focal_elements(this->mass_equivalent.get_focal_elements(), this->special_elements);
		}

		implicability(const powerset_btree<T>& m_focal_elements) : mass_aggregate<T>(m_focal_elements)
		{
			compute_values_for_mass_focal_elements(this->mass_equivalent.get_focal_elements(), this->special_elements);
		}

		implicability(const implicability<T>& b) : implicability(b.get_mass_equivalent().get_focal_elements(), b.get_special_elements())
		{}

		implicability(const powerset_btree<T>& m_focal_elements, const powerset_btree<T>& _special_elements) :
			mass_aggregate<T>(m_focal_elements, _special_elements)
		{}

		implicability(const mass_aggregate<T>& ma) : implicability(ma.get_mass_equivalent())
		{}

		implicability(const FOD& fod) : mass_aggregate<T>(fod)
		{
			compute_values_for_mass_focal_elements(this->mass_equivalent.get_focal_elements(), this->special_elements);
		}

		implicability(const FOD& fod, const Special_case s_case) : mass_aggregate<T>(fod, s_case)
		{
			compute_values_for_mass_focal_elements(this->mass_equivalent.get_focal_elements(), this->special_elements);
		}


		static T compute_aggregation_at_emptyset(const powerset_btree<T>& m_focal_elements) {
			return m_focal_elements.sub_fod_of_size(0)->value;
		}

		static T compute_aggregation_at_fod(const powerset_btree<T>& m_focal_elements) {
			T sum = 0;
			const std::vector<set_N_value<T>* >& subsets = m_focal_elements.elements();

			for (size_t i = 0; i < subsets.size(); ++i) {
					sum += subsets[i]->value;
			}
			return sum;
		}

		static T compute_aggregation(const powerset_btree<T>& m_focal_elements, const boost::dynamic_bitset<>& set) {
			T sum = 0;
			const std::vector<set_N_value<T>* >& subsets = m_focal_elements.subsets_of(set);

			for (size_t i = 0; i < subsets.size(); ++i) {
					sum += subsets[i]->value;
			}
			return sum;
		}

		static T compute_aggregation(const powerset_btree<T>& m_focal_elements, const std::vector<fod_element*>& fod_elements) {
			return compute_aggregation(m_focal_elements, m_focal_elements.fod->to_set(fod_elements));
		}


		template <class fusion_rule>
		implicability<T> apply(const fusion_rule fusion, const implicability<T>& i2) const {
			return fusion(*this, i2);
		}

		static void compute_values_for_mass_focal_elements(const powerset_btree<T>& m_focal_elements, powerset_btree<T>& special_elements) {
			const std::vector<set_N_value<T>* >& elements = m_focal_elements.elements();
			// pre-calculation for all focal elements
			for (size_t i = 0; i < elements.size(); ++i) {
				special_elements.insert(elements[i]->set, compute_aggregation(m_focal_elements, elements[i]->set));
			}
		}

		static void to_mass_focal_elements(const powerset_btree<T>& bel_tree, powerset_btree<T>& m_tree) {

			m_tree.copy(bel_tree);

			std::unordered_map<size_t, std::vector<set_N_value<T>* > > elements_by_cardinality = m_tree.elements_by_set_cardinality();
			const std::vector<std::vector<set_N_value<T>* >* >& ordered_vector = m_tree.get_vector_of_vectors_ordered_by_cardinality(elements_by_cardinality);

			// computation based on f_elements
			for (size_t c = 0; c < ordered_vector.size()-1; ++c) {
				for (size_t i = 0; i < ordered_vector[c]->size(); ++i) {
					const std::vector<set_N_value<T>* >& supersets = m_tree.strict_supersets_of((*ordered_vector[c])[i]->set);
					for (size_t k = 0; k < supersets.size(); ++k) {
						supersets[k]->value -= (*ordered_vector[c])[i]->value;
					}
				}
			}
		}
	};

} // namespace ow_bft

#endif // OW_BFT_IMPLICABILITY_HPP
