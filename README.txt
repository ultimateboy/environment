$Id$

ENVIRONMENT

Creates an api for specifying an environment for a site instance.  Other modules
may then change their logic depending upon the current active environment. When
switching environments a hook is invoked to allow each module to run additional
required actions.

The environment states are completely customizable and may be arbitrarily set
to any value that makes sense for the project at hand. Predefined environments
are 'development' and 'production'.


Example
-------

 drush env-switch development              Switch the environment to
                                           development
 drush env-switch --force production       Force the environment to switch to
                                           production even if the current
                                           environment already is production
 drush env                                 Shows current environment


Hooks Invoked
-------------

hook_environment_switch($target_env, $current_env)


