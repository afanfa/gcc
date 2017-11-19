! { dg-do run }
! { dg-options "-fcoarray=single" }
!
! Tests if change team worked
!
  use iso_fortran_env, only : team_type
  implicit none
  type(team_type) :: team
  integer :: orig_i, new_i

  orig_i = this_image ()
  form team (orig_i + 1,team)
  change team (team)
  new_i = this_image ()
  if ( orig_i + 1 .NE. new_i ) call abort
end
