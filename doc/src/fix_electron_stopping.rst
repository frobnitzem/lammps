.. index:: fix electron/stopping

fix electron/stopping command
=============================

Syntax
""""""


.. parsed-literal::

   fix ID group-ID electron/stopping Ecut file keyword value ...

* ID, group-ID are documented in :doc:`fix <fix>` command
* electron/stopping = style name of this fix command
* Ecut = minimum kinetic energy for electronic stopping (energy units)
* file = name of the file containing the electronic stopping power table
* zero or more keyword/value pairs may be appended to args
* keyword = *region* or *minneigh*
  
  .. parsed-literal::
  
       *region* value = region-ID
         region-ID = region, whose atoms will be affected by this fix
       *minneigh* value = minneigh
         minneigh = minimum number of neighbors an atom to have stopping applied



Examples
""""""""


.. parsed-literal::

   fix el all electron/stopping 10.0 elstop-table.txt
   fix el all electron/stopping 10.0 elstop-table.txt minneigh 3
   fix el mygroup electron/stopping 1.0 elstop-table.txt region bulk

Description
"""""""""""

This fix implements inelastic energy loss for fast projectiles in solids. It
applies a friction force to fast moving atoms to slow them down due to
:ref:`electronic stopping <elstopping>` (energy lost via electronic collisions per
unit of distance). This fix should be used for simulation of irradiation
damage or ion implantation, where the ions can lose noticeable amounts of
energy from electron excitations. If the electronic stopping power is not
considered, the simulated range of the ions can be severely overestimated
(:ref:`Nordlund98 <Nordlund98>`, :ref:`Nordlund95 <Nordlund95>`).

The electronic stopping is implemented by applying a friction force
to each atom as:


.. math::

   \begin{equation}\vec{F}_i = \vec{F}^0_i - \frac{\vec{v}_i}{\|\vec{v}_i\|} \cdot S_e\end{equation}

where :math:`\vec{F}_i` is the resulting total force on the atom.
:math:`\vec{F}^0_i` is the original force applied to the atom, :math:`\vec{v}_i` is
its velocity and :math:`S_e` is the stopping power of the ion.

.. note::

   In addition to electronic stopping, atomic cascades and irradiation
   simulations require the use of an adaptive timestep (see
   :doc:`fix dt/reset <fix_dt_reset>`) and the repulsive ZBL potential (see
   :doc:`ZBL <pair_zbl>` potential) or similar. Without these settings the
   interaction between the ion and the target atoms will be faulty. It is also
   common to use in such simulations a thermostat (:doc:`fix\_nvt <fix_nh>`) in
   the borders of the simulation cell.

.. note::

   This fix removes energy from fast projectiles without depositing it as a
   heat to the simulation cell. Such implementation might lead to the unphysical
   results when the amount of energy deposited to the electronic system is large,
   e.g. simulations of Swift Heavy Ions (energy per nucleon of 100 keV/amu or
   higher) or multiple projectiles. You could compensate energy loss by coupling
   bulk atoms with some thermostat or control heat transfer between electronic and
   atomic subsystems with the two-temperature model (:doc:`fix\_ttm <fix_ttm>`).

At low velocities the electronic stopping is negligible. The electronic
friction is not applied to atoms whose kinetic energy is smaller than *Ecut*\ ,
or smaller than the lowest energy value given in the table in *file*\ .
Electronic stopping should be applied only when a projectile reaches bulk
material. This fix scans neighbor list and excludes atoms with fewer than
*minneigh* neighbors (by default one). If the pair potential cutoff is large,
minneigh should be increased, though not above the number of nearest neighbors
in bulk material. An alternative is to disable the check for neighbors by
setting *minneigh* to zero and using the *region* keyword. This is necessary
when running simulations of cluster bombardment.

If the *region* keyword is used, the atom must also be in the specified
geometric :doc:`region <region>` in order to have electronic stopping applied to
it. This is useful if the position of the bulk material is fixed. By default
the electronic stopping is applied everywhere in the simulation cell.


----------


The energy ranges and stopping powers are read from the file *file*\ .
Lines starting with *#* and empty lines are ignored. Otherwise each
line must contain exactly **N+1** numbers, where **N** is the number of atom
types in the simulation.

The first column is the energy for which the stopping powers on that
line apply. The energies must be sorted from the smallest to the largest.
The other columns are the stopping powers :math:`S_e` for each atom type,
in ascending order, in force :doc:`units <units>`. The stopping powers for
intermediate energy values are calculated with linear interpolation between
2 nearest points.

For example:


.. parsed-literal::

   # This is a comment
   #       atom-1    atom-2
   # eV    eV/Ang    eV/Ang  # units metal
    10        0        0
   250       60       80
   750      100      150

If an atom which would have electronic stopping applied to it has a
kinetic energy higher than the largest energy given in *file*\ , LAMMPS
will exit with an error message.

The stopping power depends on the energy of the ion and the target
material. The electronic stopping table can be obtained from
scientific publications, experimental databases or by using
:ref:`SRIM <SRIM>` software. Other programs such as :ref:`CasP <CasP>` or
:ref:`PASS <PASS>` can calculate the energy deposited as a function
of the impact parameter of the ion; these results can be used
to derive the stopping power.

**Restart, fix\_modify, output, run start/stop, minimize info:**

No information about this fix is written to :doc:`binary restart files <restart>`.

The :doc:`fix\_modify <fix_modify>` options are not supported.

This fix computes a global scalar, which can be accessed by various
:doc:`output commands <Howto_output>`. The scalar is the total energy
loss from electronic stopping applied by this fix since the start of
the latest run. It is considered "intensive".

The *start/stop* keywords of the :doc:`run <run>` command have no effect
on this fix.

Restrictions
""""""""""""


This pair style is part of the USER-MISC package. It is only enabled if
LAMMPS was built with that package. See the :doc:`Build package <Build_package>`
doc page for more info.

Default
"""""""

The default is no limitation by region, and minneigh = 1.


----------


.. _elstopping:



**(electronic stopping)** Wikipedia - Electronic Stopping Power: https://en.wikipedia.org/wiki/Stopping\_power\_%28particle\_radiation%29

.. _Nordlund98:



**(Nordlund98)** Nordlund, Kai, et al.  Physical Review B 57.13 (1998): 7556.

.. _Nordlund95:



**(Nordlund95)** Nordlund, Kai. Computational materials science 3.4 (1995): 448-456.

.. _SRIM:



**(SRIM)** SRIM webpage: http://www.srim.org/

.. _CasP:



**(CasP)** CasP webpage: https://www.helmholtz-berlin.de/people/gregor-schiwietz/casp\_en.html

.. _PASS:



**(PASS)** PASS webpage: https://www.sdu.dk/en/DPASS


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Commands_all.html
