#!/bin/bash

drush en bootstrap -y;
drush en culturefeed_bootstrap -y;
drush vset theme_default culturefeed_bootstrap -y;
drush en jquery_update -y;
