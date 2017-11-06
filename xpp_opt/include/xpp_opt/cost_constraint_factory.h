/**
 @file    constraint_factory.h
 @author  Alexander W. Winkler (winklera@ethz.ch)
 @date    Jul 19, 2016
 @brief   Declares factory class to build constraints.
 */

#ifndef XPP_XPP_OPT_INCLUDE_XPP_OPT_COST_CONSTRAINT_FACTORY_H_
#define XPP_XPP_OPT_INCLUDE_XPP_OPT_COST_CONSTRAINT_FACTORY_H_

#include <memory>
#include <string>
#include <vector>

#include <xpp_solve/composite.h>

#include <xpp_opt/height_map.h>
#include <xpp_opt/models/robot_model.h>
#include <xpp_opt/optimization_parameters.h>
#include <xpp_states/cartesian_declarations.h>
#include <xpp_states/endeffectors.h>
#include <xpp_states/state.h>



namespace xpp {


/** Builds all types of constraints/costs for the user.
  *
  * Implements the factory method, hiding object creation from the client.
  * The client specifies which object it wants, and this class is responsible
  * for the object creation. Factory method is like template method pattern
  * for object creation.
  */
class CostConstraintFactory {
public:
  using ComponentPtr     = std::shared_ptr<opt::Component>;
  using OptVarsContainer = std::shared_ptr<opt::Composite>;
  using MotionParamsPtr  = std::shared_ptr<OptimizationParameters>;
  using Derivatives      = std::vector<MotionDerivative>;

  CostConstraintFactory ();
  virtual ~CostConstraintFactory ();

  void Init(const OptVarsContainer&,
            const MotionParamsPtr&,
            const HeightMap::Ptr& terrain,
            const RobotModel& model,
            const EndeffectorsPos& ee_pos,
            const State3dEuler& initial_base,
            const State3dEuler& final_base);

  ComponentPtr GetCost(const CostName& id, double weight) const;
  ComponentPtr GetConstraint(ConstraintName name) const;

private:
  MotionParamsPtr params;
  OptVarsContainer opt_vars_;
  HeightMap::Ptr terrain_;
  RobotModel model_;


  EndeffectorsPos initial_ee_W_;
  State3dEuler initial_base_;
  State3dEuler final_base_;


  // constraints
  ComponentPtr MakeStateConstraint() const;
  ComponentPtr MakeDynamicConstraint() const;
  ComponentPtr MakeRangeOfMotionBoxConstraint() const;
  ComponentPtr MakeTotalTimeConstraint() const;
  ComponentPtr MakeTerrainConstraint() const;
  ComponentPtr MakeForceConstraint() const;
  ComponentPtr MakeSwingConstraint() const;
  ComponentPtr MakeBaseRangeOfMotionConstraint() const;

  // costs
  ComponentPtr MakeForcesCost(double weight) const;
  ComponentPtr MakeMotionCost(double weight) const;
  ComponentPtr MakePolynomialCost(const std::string& poly_id,
                                   const Vector3d& weight_dimensions,
                                   double weight) const;

  ComponentPtr ToCost(const ComponentPtr& constraint, double weight) const;

  std::vector<EndeffectorID> GetEEIDs() const { return initial_ee_W_.GetEEsOrdered(); };
};

} /* namespace xpp */

#endif /* XPP_XPP_OPT_INCLUDE_XPP_OPT_COST_CONSTRAINT_FACTORY_H_ */
