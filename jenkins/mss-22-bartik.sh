#!/bin/bash

drush en bartik -y;
drush vset theme_default bartik -y;
drush dis jquery_update -y;
