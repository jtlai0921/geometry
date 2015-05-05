#ifndef BOOST_GEOMETRY_PROJECTIONS_POLY_HPP
#define BOOST_GEOMETRY_PROJECTIONS_POLY_HPP

// Boost.Geometry - extensions-gis-projections (based on PROJ4)
// This file is automatically generated. DO NOT EDIT.

// Copyright (c) 2008-2015 Barend Gehrels, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// This file is converted from PROJ4, http://trac.osgeo.org/proj
// PROJ4 is originally written by Gerald Evenden (then of the USGS)
// PROJ4 is maintained by Frank Warmerdam
// PROJ4 is converted to Boost.Geometry by Barend Gehrels

// Last updated version of proj: 4.9.1

// Original copyright notice:
 
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.


#include <boost/geometry/extensions/gis/projections/impl/base_static.hpp>
#include <boost/geometry/extensions/gis/projections/impl/base_dynamic.hpp>
#include <boost/geometry/extensions/gis/projections/impl/projects.hpp>
#include <boost/geometry/extensions/gis/projections/impl/factory_entry.hpp>
#include <boost/geometry/extensions/gis/projections/impl/pj_mlfn.hpp>
#include <boost/geometry/extensions/gis/projections/impl/pj_msfn.hpp>

namespace boost { namespace geometry { namespace projections
{
    #ifndef DOXYGEN_NO_DETAIL
    namespace detail { namespace poly{
            static const double TOL = 1e-10;
            static const double CONV = 1e-10;
            static const int N_ITER = 10;
            static const int I_ITER = 20;
            static const double ITOL = 1.e-12;

            struct par_poly
            {
                double ml0;
                double en[EN_SIZE];
            };

            // template class, using CRTP to implement forward/inverse
            template <typename Geographic, typename Cartesian, typename Parameters>
            struct base_poly_ellipsoid : public base_t_fi<base_poly_ellipsoid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>
            {

                 typedef double geographic_type;
                 typedef double cartesian_type;

                par_poly m_proj_parm;

                inline base_poly_ellipsoid(const Parameters& par)
                    : base_t_fi<base_poly_ellipsoid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>(*this, par) {}

                inline void fwd(geographic_type& lp_lon, geographic_type& lp_lat, cartesian_type& xy_x, cartesian_type& xy_y) const
                {
                    double  ms, sp, cp;

                    if (fabs(lp_lat) <= TOL) { xy_x = lp_lon; xy_y = -this->m_proj_parm.ml0; }
                    else {
                        sp = sin(lp_lat);
                        ms = fabs(cp = cos(lp_lat)) > TOL ? pj_msfn(sp, cp, this->m_par.es) / sp : 0.;
                        xy_x = ms * sin(lp_lon *= sp);
                        xy_y = (pj_mlfn(lp_lat, sp, cp, this->m_proj_parm.en) - this->m_proj_parm.ml0) + ms * (1. - cos(lp_lon));
                    }
                }

                inline void inv(cartesian_type& xy_x, cartesian_type& xy_y, geographic_type& lp_lon, geographic_type& lp_lat) const
                {
                    xy_y += this->m_proj_parm.ml0;
                    if (fabs(xy_y) <= TOL) { lp_lon = xy_x; lp_lat = 0.; }
                    else {
                        double r, c, sp, cp, s2ph, ml, mlb, mlp, dPhi;
                        int i;

                        r = xy_y * xy_y + xy_x * xy_x;
                        for (lp_lat = xy_y, i = I_ITER; i ; --i) {
                            sp = sin(lp_lat);
                            s2ph = sp * ( cp = cos(lp_lat));
                            if (fabs(cp) < ITOL)
                                throw proj_exception();;
                            c = sp * (mlp = sqrt(1. - this->m_par.es * sp * sp)) / cp;
                            ml = pj_mlfn(lp_lat, sp, cp, this->m_proj_parm.en);
                            mlb = ml * ml + r;
                            mlp = this->m_par.one_es / (mlp * mlp * mlp);
                            lp_lat += ( dPhi =
                                ( ml + ml + c * mlb - 2. * xy_y * (c * ml + 1.) ) / (
                                this->m_par.es * s2ph * (mlb - 2. * xy_y * ml) / c +
                                2.* (xy_y - ml) * (c * mlp - 1. / s2ph) - mlp - mlp ));
                            if (fabs(dPhi) <= ITOL)
                                break;
                        }
                        if (!i)
                            throw proj_exception();;
                        c = sin(lp_lat);
                        lp_lon = asin(xy_x * tan(lp_lat) * sqrt(1. - this->m_par.es * c * c)) / sin(lp_lat);
                    }
                }
            };

            // template class, using CRTP to implement forward/inverse
            template <typename Geographic, typename Cartesian, typename Parameters>
            struct base_poly_spheroid : public base_t_fi<base_poly_spheroid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>
            {

                 typedef double geographic_type;
                 typedef double cartesian_type;

                par_poly m_proj_parm;

                inline base_poly_spheroid(const Parameters& par)
                    : base_t_fi<base_poly_spheroid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>(*this, par) {}

                inline void fwd(geographic_type& lp_lon, geographic_type& lp_lat, cartesian_type& xy_x, cartesian_type& xy_y) const
                {
                    double  cot, E;

                    if (fabs(lp_lat) <= TOL) { xy_x = lp_lon; xy_y = this->m_proj_parm.ml0; }
                    else {
                        cot = 1. / tan(lp_lat);
                        xy_x = sin(E = lp_lon * sin(lp_lat)) * cot;
                        xy_y = lp_lat - this->m_par.phi0 + cot * (1. - cos(E));
                    }
                }

                inline void inv(cartesian_type& xy_x, cartesian_type& xy_y, geographic_type& lp_lon, geographic_type& lp_lat) const
                {
                    double B, dphi, tp;
                    int i;

                    if (fabs(xy_y = this->m_par.phi0 + xy_y) <= TOL) { lp_lon = xy_x; lp_lat = 0.; }
                    else {
                        lp_lat = xy_y;
                        B = xy_x * xy_x + xy_y * xy_y;
                        i = N_ITER;
                        do {
                            tp = tan(lp_lat);
                            lp_lat -= (dphi = (xy_y * (lp_lat * tp + 1.) - lp_lat -
                                .5 * ( lp_lat * lp_lat + B) * tp) /
                                ((lp_lat - xy_y) / tp - 1.));
                        } while (fabs(dphi) > CONV && --i);
                        if (! i) throw proj_exception();;
                        lp_lon = asin(xy_x * tan(lp_lat)) / sin(lp_lat);
                    }
                }
            };

            // Polyconic (American)
            template <typename Parameters>
            void setup_poly(Parameters& par, par_poly& proj_parm)
            {
                if (par.es) {
                    if (!pj_enfn(par.es, proj_parm.en)) throw proj_exception(0);
                    proj_parm.ml0 = pj_mlfn(par.phi0, sin(par.phi0), cos(par.phi0), proj_parm.en);
                } else {
                    proj_parm.ml0 = -par.phi0;
                }
            }

        }} // namespace detail::poly
    #endif // doxygen

    /*!
        \brief Polyconic (American) projection
        \ingroup projections
        \tparam Geographic latlong point type
        \tparam Cartesian xy point type
        \tparam Parameters parameter type
        \par Projection characteristics
         - Conic
         - Spheroid
         - Ellipsoid
        \par Example
        \image html ex_poly.gif
    */
    template <typename Geographic, typename Cartesian, typename Parameters = parameters>
    struct poly_ellipsoid : public detail::poly::base_poly_ellipsoid<Geographic, Cartesian, Parameters>
    {
        inline poly_ellipsoid(const Parameters& par) : detail::poly::base_poly_ellipsoid<Geographic, Cartesian, Parameters>(par)
        {
            detail::poly::setup_poly(this->m_par, this->m_proj_parm);
        }
    };

    /*!
        \brief Polyconic (American) projection
        \ingroup projections
        \tparam Geographic latlong point type
        \tparam Cartesian xy point type
        \tparam Parameters parameter type
        \par Projection characteristics
         - Conic
         - Spheroid
         - Ellipsoid
        \par Example
        \image html ex_poly.gif
    */
    template <typename Geographic, typename Cartesian, typename Parameters = parameters>
    struct poly_spheroid : public detail::poly::base_poly_spheroid<Geographic, Cartesian, Parameters>
    {
        inline poly_spheroid(const Parameters& par) : detail::poly::base_poly_spheroid<Geographic, Cartesian, Parameters>(par)
        {
            detail::poly::setup_poly(this->m_par, this->m_proj_parm);
        }
    };

    #ifndef DOXYGEN_NO_DETAIL
    namespace detail
    {

        // Factory entry(s)
        template <typename Geographic, typename Cartesian, typename Parameters>
        class poly_entry : public detail::factory_entry<Geographic, Cartesian, Parameters>
        {
            public :
                virtual projection<Geographic, Cartesian>* create_new(const Parameters& par) const
                {
                    if (par.es)
                        return new base_v_fi<poly_ellipsoid<Geographic, Cartesian, Parameters>, Geographic, Cartesian, Parameters>(par);
                    else
                        return new base_v_fi<poly_spheroid<Geographic, Cartesian, Parameters>, Geographic, Cartesian, Parameters>(par);
                }
        };

        template <typename Geographic, typename Cartesian, typename Parameters>
        inline void poly_init(detail::base_factory<Geographic, Cartesian, Parameters>& factory)
        {
            factory.add_to_factory("poly", new poly_entry<Geographic, Cartesian, Parameters>);
        }

    } // namespace detail
    #endif // doxygen

}}} // namespace boost::geometry::projections

#endif // BOOST_GEOMETRY_PROJECTIONS_POLY_HPP

