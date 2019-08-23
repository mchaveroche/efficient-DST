#ifndef EFFICIENT_DST_BELIEF_HPP
#define EFFICIENT_DST_BELIEF_HPP

#include <implicability.hpp>
#include <mobius_aggregate.hpp>

namespace efficient_DST{

	template <typename T = double>
	class belief : public mobius_aggregate<T> {
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

		belief(const mass<T>& m) : mobius_aggregate<T>(m)
		{
			compute_values_for_mass_focal_elements(this->mass_equivalent.get_focal_elements(), this->special_elements);
		}

		belief(const powerset_btree<T>& m_focal_elements) : mobius_aggregate<T>(m_focal_elements)
		{
			compute_values_for_mass_focal_elements(this->mass_equivalent.get_focal_elements(), this->special_elements);
		}

		belief(const belief<T>& bel) : belief(bel.get_mass_equivalent().get_focal_elements(), bel.get_special_elements())
		{}

		belief(const powerset_btree<T>& m_focal_elements, const powerset_btree<T>& _special_elements) :
			mobius_aggregate<T>(m_focal_elements, _special_elements)
		{}

		belief(const mobius_aggregate<T>& ma) : belief(ma.get_mass_equivalent())
		{}

		belief(const FOD& fod) : mobius_aggregate<T>(fod)
		{
			compute_values_for_mass_focal_elements(this->mass_equivalent.get_focal_elements(), this->special_elements);
		}

		belief(const FOD& fod, const special_case_t s_case) : mobius_aggregate<T>(fod, s_case)
		{
			compute_values_for_mass_focal_elements(this->mass_equivalent.get_focal_elements(), this->special_elements);
		}


		static T compute_aggregation_at_emptyset(const powerset_btree<T>& m_focal_elements) {
			return 0;
		}

		static T compute_aggregation_at_fod(const powerset_btree<T>& m_focal_elements) {
			T sum = implicability<T>::compute_aggregation_at_fod(m_focal_elements);

			const set_N_value<T>* emptyset = m_focal_elements.sub_fod_of_size(0);
			if(emptyset)
				sum -= emptyset->value;

			return sum;
		}

		static T compute_aggregation(const powerset_btree<T>& m_focal_elements, const boost::dynamic_bitset<>& set) {
			T sum = implicability<T>::compute_aggregation(m_focal_elements, set);

			const set_N_value<T>* emptyset = m_focal_elements.sub_fod_of_size(0);
			if(emptyset)
				sum -= emptyset->value;

			return sum;
		}

		static T compute_aggregation(const powerset_btree<T>& m_focal_elements, const std::vector<fod_element*>& fod_elements) {
			return compute_aggregation(m_focal_elements, m_focal_elements.fod->to_set(fod_elements));
		}


		template <class fusion_rule>
		const belief<T> apply(const fusion_rule fusion, const belief<T>& b2) const {
			return fusion(*this, b2);
		}

		static void compute_values_for_mass_focal_elements(const powerset_btree<T>& m_focal_elements, powerset_btree<T>& special_elements) {
			const std::vector<set_N_value<T>* >& elements = m_focal_elements.elements();
			// pre-calculation for all focal elements
			for (size_t i = 0; i < elements.size(); ++i) {
				special_elements.insert(elements[i]->set, compute_aggregation(m_focal_elements, elements[i]->set));
			}
		}

		static void to_mass_focal_elements(const powerset_btree<T>& bel_tree, powerset_btree<T>& m_tree) {
			implicability<T>::to_mass_focal_elements(bel_tree, m_tree);
		}
	};

} // namespace efficient_DST

#endif // EFFICIENT_DST_BELIEF_HPP
