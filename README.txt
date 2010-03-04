$Id$

ENVIRONMENT

Creates a drush command line interface for setting an environment for a site
instance.  Other modules may then change their logic depending upon the current
active environment.  The 'drush env switch' command also invokes a hook to allow
each module to run additional required actions when switching environments.

The environment states are completely customizable and may be arbitrarily set
to any value that makes sense for the project at hand.  The recommended set of
environment states are 'development', 'production', and 'readonly'.


Example
-------

 drush env switch development              Switch the environment to
                                           development
 drush env switch production force         Force the environment to switch to
                                           production even if the current
                                           environment already is production


Hooks Invoked
-------------

hook_environment_switch($target_env, $current_env)


Additional Behaviors
--------------------

Drush:
  - drush env switch
    Invokes switching the environment
