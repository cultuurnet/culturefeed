culturefeed
===========

[![Build Status](https://travis-ci.org/cultuurnet/culturefeed.svg?branch=master)](https://travis-ci.org/jonschlinkert/remarkable)

Drupal module suite for building an event site based on events gathered in an external backoffice "outdatabase" (UiTdatabank), but with the extra tools you can do a lot more. But for this version you can only use it __having a key and secret from the UiTdatabank__ or use the demo key at [http://tools.uitdatabank.be/docs/search-api-v2-getting-started](http://tools.uitdatabank.be/docs/search-api-v2-getting-started).

__[Live demo connecting production API](http://www.culturefeed.be/)__

__[Live demo connecting acceptance API](http://acc.culturefeed.be/)__



## Culturefeed-kickstart

When you start with a clean Drupal install of just for setting up a quick demo site we created a __[Drupal install profile](https://drupal.org/developing/distributions)__ ("Installation profiles provide specific site features and functions for a specific purpose or type of site distributions"). This included also a shell script (Build.sh) which downloads:

- drupal core (at the moment 7.32) 
- drupal contribs (bootstrap 3.0)
- culturefeed
- culturefeed_bootstrap
- vendor/cultuurnet/cdb
- vendor/cultuurnet/auth
- vendor/cultuurnet/search
- vendor/cultuurnet/culturefeed-php
- lessphp


## Install

Prerequisites:

- git installed
- drush installed
- composer installed (http://getcomposer.org/doc/00-intro.md#downloading-the-composer-executable)

Place the module suite in your __sites/\*/modules__ folder, you can do this with GIT or download it [here](https://github.com/cultuurnet/culturefeed/releases).

```bash
git clone https://github.com/cultuurnet/culturefeed
```
Afterwards copy the composer.json file from the root of the module suite to the root directory of your Drupal. If you use allready composer.json you have to add the required libraries.

```
{
    "name": "cultuurnet/culturefeed",
    "type": "drupal-module",
    "description": "CultuurNet culturefeed PHP library & Drupal module",
    "license": "Apache-2.0",
    "require": {
      "cultuurnet/search": "~1.0",
      "cultuurnet/cdb": "~2.0",
      "cultuurnet/culturefeed-php": "~1.0"
    },
    "minimum-stability": "stable"
}
```

After that run ``composer install`` or ``composer update`` if already using composer. This will donwload our libraries and add theme to a vendor directory. It will also create a composer.lock file. Typical we add vendor/* to the .gitignore file.

Last step is include this in your settings.php file of your site.

```
require 'vendor/autoload.php';
```

You should be able now to 


## Tutorials 


## PHP libraries

Most of the modules have an dependency to these PHP libraries. See Install how to install theme with composer.

###CultuurNet\Cdb 
Fluent PHP library for manipulating, serializing and deserializing data present in CultuurNet's CdbXML 3.2 format

[https://github.com/cultuurnet/cdb](https://github.com/cultuurnet/cdb)

###CultuurNet\Search
Php library for creating SOLR queries (basic SOLR queries and custom business logic)

[https://github.com/cultuurnet/Search](https://github.com/cultuurnet/Search)

###CultuurNet\Auth
The consumer-side of the authentication flow of CultuurNet's UiTID, which is based on OAuth 1.0a Core
solid base for consumers of various OAuth-protected resources provided by CultuurNet

[https://github.com/cultuurnet/Auth ](https://github.com/cultuurnet/Auth )

###CultuurNet\CultureFeed-PHP
Integration of all UiTID API calls.

[https://github.com/cultuurnet/culturefeed-php](https://github.com/cultuurnet/culturefeed-php)


## Culturefeed_bootstrap

Culturefeed Bootstrap is a responsive, mobile first __subtheme of the Bootstrap Framework__. 

### Customize it
Culturefeed Bootstrap has some basic theme settings (admin/appearance/settings/culturefeed_bootstrap) where you can customize branding colors, font-family, border-radius, navbars, etc. to your own needs. If you are a Drupal themer you can - of course - change all possible less variables and bootstrap overrides or kick everything out and build your own theme :-)

### Bootstrap 3.0.2
If you like the Culturefeed Bootstrap theme or plan to build your own [Bootstrap subtheme](https://www.drupal.org/project/bootstrap) some knowledge about the [Bootstrap Framework](http://getbootstrap.com/) (version 3.0.2) will come in handy. Good to know: Culturefeed Bootstrap uses [Less](https://www.drupal.org/project/less) to compile the CSS.

###  Font Awesome 4.0.3
We chose to make use of the [Font Awesome Icon library](http://fortawesome.github.io/Font-Awesome/) (version 4.0.3) instead of the Glyphicons that are built in with Bootstrap. Here's an overview of all icons and examples.


## Modules

Please switch on only the modules you need.

### culturefeed core
Core of the module suite and is required by the other modules. It provides the settings form where you can enter the API Information.

__[More info at wiki page](https://github.com/cultuurnet/culturefeed/wiki/Culturefeed)__

### culturefeed_search
Base framework to enable searches on your site. Out of the box this module doesn't provide any interface elements (use Culturefeed Search UI and Culturefeed Agenda instead). It's provides some drush commands and caching.

__[More info at wiki page](https://github.com/cultuurnet/culturefeed/wiki/Culturefeed-Search)__

### culturefeed_agenda
Provides a Culturefeed search page available on 'agenda/search' and detail pages.  The blocks provided by this module can be used to extend the detail pages of events, actors and productions.
Includes also a simple search form.

__[More info at wiki page](https://github.com/cultuurnet/culturefeed/wiki/Culturefeed-Agenda)__

### culturefeed_search_ui
Basic elements needed to build up an event search (such as provided by Culturefeed Agenda). 

- basic search form with type selector in the front (can be extended)
- sort block
- active filters
- facets

__[More info at wiki page](https://github.com/cultuurnet/culturefeed/wiki/Culturefeed-Search-UI)__

To integrate a search page we also wrote a [Tutorial](https://github.com/cultuurnet/culturefeed/wiki/Tutorial-Search-page)

### culturefeed_search_views
Views integration to list events, actors or productions. To create culturefeed views, create a new view and configure it to show Cdb items. Once the view is created, following handlers can be set:

- Fields
- Filters
- Sort

__[More info at wiki page](https://github.com/cultuurnet/culturefeed/wiki/Culturefeed-Search-UI)__

### culturefeed_devel
This module logs every query to our API, on the screen for admins or to the watchdog. Handy tool to check which is the performance killer, your Drupal install or our API's.


### culturefeed_entry_ui
Functions to do a CRUD on events are already in library, but we are building a form to create, update and delete events in our UiTdatabank as well.

__[More info at wiki page](https://github.com/cultuurnet/culturefeed/wiki/Culturefeed-Entry-UI)__

### culturefeed_pages
User can create a page (locations, performers, families, schools, …) or become member and follower. 

__Wiki page not yet available__


### culturefeed_ui
Provides a collection of pages and blocks to enhance the user pages with __UiTID__ information. To do this it will override and/or enhance the default Drupal user pages. Some features:

- edit profile page
- edit account page
- search users page
- caching
- teaser (formerly connect)
- most active user
- login (formerly profile box)
- profile menu
- e-mail confirmation reminder
- search users
- profile summary (formerly My UiTiD)

__[More info at wiki page](https://github.com/cultuurnet/culturefeed/wiki/Culturefeed-UI)__

To integrate UiTID we also wrote a [Tutorial](https://github.com/cultuurnet/culturefeed/wiki/Tutorial-CultureFeedUI)

### culturefeed_social
All ‘social’ features: likes, comments, attends, etc.

__[More info at wiki page](https://github.com/cultuurnet/culturefeed/wiki/Culturefeed-Social)__

### culturefeed_uitpas
Integration with the UiTPAS card system. It contains multiple pages and blocks that can be used to build an interface that allows:

- easy access to the advantages and promotions a passholder has access to,
- possibility to register your UiTPAS,
- highlights of promotions and actions.

__[More info at wiki page](https://github.com/cultuurnet/culturefeed/wiki/Culturefeed-UiTPAS)__

To integrate UiTPAS we also wrote a [Tutorial](https://github.com/cultuurnet/culturefeed/wiki/Tutorial-UiTPAS)



### culturefeed_mailing
To create mailings with search results from event (based on lifestyleprofile, Vlieg weekendflash, …). __Not yet available for partners.__ Please contact us if interested. 


### culturefeed_messages
Send messages to other users (most likely page owners), interesting when actors become pages.


### culturefeed_userpoints_ui
Collect on line userpoints (for specific actions like writing reviews) and claim promotions (Vlieg).  __Not yet available for partners.__ Please contact us if interested. 

### culturefeed_roles
Pre-assign roles to UiTID users that have not logged in yet.  Drush integration.




## License

[Apache-2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
