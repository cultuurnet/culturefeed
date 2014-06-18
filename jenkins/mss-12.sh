#!/bin/bash

drush vset culturefeed_gtm_container_id GTM-KDH3DK;
drush scr sites/all/modules/culturefeed/jenkins/mss-12.php;