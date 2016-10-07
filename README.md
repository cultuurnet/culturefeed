Culturefeed
===========

[![Build Status](https://travis-ci.org/cultuurnet/culturefeed.svg?branch=master)](https://api.travis-ci.org/cultuurnet/culturefeed.svg?branch=master)

Drupal module suite for building an event site based on events gathered in an external backoffice "outdatabase" (UiTdatabank), but with the extra tools you can do a lot more. For this version you can only use it __having a key and secret from the UiTdatabank__ or use the demo key from [this page](http://tools.uitdatabank.be/docs/search-api-v2-getting-started).

__[Live demo connecting production API](http://www.culturefeed.be/) only available from 7:00 till 20:00  (Brussels timezone)__

__[Live demo connecting acceptance API](http://acc.culturefeed.be/) only available from 7:00 till 20:00  (Brussels timezone)__

| Important note   | 
|----------|
| Since the culturefeed 3.6 release __PHP v5.5__ is the minimum requirement and also the __PHP INTL extension__ is needed. To install the extension on your system take a look at [this blogpost](http://asdqwe.net/blog/enabling-installing-intl-package-php-from-terminal). |


## Culturefeed-kickstart

When you start with a clean Drupal install or just for setting up a quick demo site we created a __[Drupal install profile](https://drupal.org/developing/distributions)__ ("Installation profiles provide specific site features and functions for a specific purpose or type of site distributions"). This included also a shell script (Build.sh) which downloads:

- drupal core (at the moment 7.32) 
- drupal contribs (bootstrap 3.0)
- culturefeed
- culturefeed_bootstrap
- vendor/cultuurnet/cdb
- vendor/cultuurnet/auth
- vendor/cultuurnet/search
- vendor/cultuurnet/culturefeed-php
- vendor/cultuurnet/calendar-summary
- vendor/cultuurnet/sitemap-xml
- lessphp


## Install

Prerequisites:

- php > v5.5 and the php intl extension (see important note above) 
- git installed
- drush installed
- composer installed (http://getcomposer.org/doc/00-intro.md#downloading-the-composer-executable)

Place the module suite in your __sites/\*/modules__ folder, you can do this with GIT or download it [here](https://github.com/cultuurnet/culturefeed/releases).

```bash
git clone https://github.com/cultuurnet/culturefeed
```
Afterwards copy the composer.json file from the root of the module suite to the root directory of your Drupal. If you use already composer.json you have to add the required libraries.

```
{
    "name": "cultuurnet/culturefeed",
    "type": "drupal-module",
    "description": "CultuurNet culturefeed Drupal module",
    "license": "Apache-2.0",
    "require": {
      "cultuurnet/search": "~1.2",
      "cultuurnet/cdb": "~2.1",
      "cultuurnet/culturefeed-php": "~1.5",
      "cultuurnet/calendar-summary": "~1.0",
      "cultuurnet/sitemap-xml": "~1.0"
    },
    "require-dev": {
      "phing/phing": "~2.10"
    },
    "minimum-stability": "dev",
    "prefer-stable": true
}
```

After that run ``composer install`` or ``composer update`` if already using composer. This will download our libraries and add a theme to a vendor directory. It will also create a composer.lock file. Typical we add vendor/* to the .gitignore file.

Last step is to include autoload.php in your settings.php file of your site.

```
require 'vendor/autoload.php';
```

You should now be able to enable the modules (typically you start with culturefeed, culturefeed_ui, culturefeed_search, culturefeed_search_ui and culturefeed_agenda) and fill in your key and secret:

- culturefeed core at admin/config/culturefeed/api-settings
- if culturefeed_search enabled also at admin/config/culturefeed/search

It's a good practice to connect your user/1 (admin) user with a UiTID account:
- Log in as user/1
- Go to culturefeed/oauth/connect and login to UiTID.

If this works culturefeed core is configured well. And if you get results at the path agenda/search culturefeed_search is working as well.


## Tutorials

We created 3 tutorials to integrate the most common use cases:

- [Set up a search page](https://github.com/cultuurnet/culturefeed/wiki/Tutorial-Search-page)
- [Integrate UiTID](https://github.com/cultuurnet/culturefeed/wiki/Tutorial-CultureFeedUI)
- [Integrate UiTPAS](https://github.com/cultuurnet/culturefeed/wiki/Tutorial-UiTPAS)

## PHP libraries

Most of the modules have an dependency on these PHP libraries. See "Install" how to install theme with composer.

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

###CultuurNet\calendar-summary
The calendar-summary PHP takes a CultureFeed_Cdb_Data_Calendar object (hence the dependency on cultuurnet/cdb, and formats it. Right now there's a HTML formatter and a plain text formatter. Current options: 'lg' (for permanent events only this option is available), 'md', 'md', 'sm', 'xs'.


[https://github.com/cultuurnet/calendar-summary](https://github.com/cultuurnet/calendar-summary)

###CultuurNet\sitemap-xml-php
PHP library for writing sitemap XML conform with the sitemaps.org schema. Example see http://www.uitinvlaanderen.be/sitemap.xml. Only needed when you turn on the culturefeed_sitemap module.

[https://github.com/cultuurnet/sitemap-xml-php](https://github.com/cultuurnet/sitemap-xml-php)


## Theme

__Culturefeed Bootstrap__ is the only supported and recommended base theme for Culturefeed. It's a responsive subtheme of the Bootstrap Framework and has it's own subtheme to start from. But of course you can implement Culturefeed in your prefered theme as well.

[https://github.com/cultuurnet/culturefeed_bootstrap](https://github.com/cultuurnet/culturefeed_bootstrap)


## Modules

Please enable only the modules you need.

### Culturefeed core

Core of the module suite and is required by the other modules. It provides the settings form where you can enter the API Information.

__[More info at wiki page](https://github.com/cultuurnet/culturefeed/wiki/Culturefeed)__

### Culturefeed_search

Base framework to enable searches on your site. Out of the box this module doesn't provide any interface elements (use Culturefeed Search UI and Culturefeed Agenda instead). It provides some drush commands and caching.

__[More info at wiki page](https://github.com/cultuurnet/culturefeed/wiki/Culturefeed-Search)__

### Culturefeed_agenda

Provides a Culturefeed search page available on 'agenda/search' and detail pages.  The blocks provided by this module can be used to extend the detail pages of events, actors and productions.
Includes also a simple search form.

__[More info at wiki page](https://github.com/cultuurnet/culturefeed/wiki/Culturefeed-Agenda)__

### Culturefeed_search_ui
Basic elements to build up an event search (such as provided by Culturefeed Agenda). 

- Basic search form with type selector in the front (can be extended)
- Sort block
- Active filters
- Facets

__[More info at wiki page](https://github.com/cultuurnet/culturefeed/wiki/Culturefeed-Search-UI)__

To integrate a search page we also wrote a [tutorial](https://github.com/cultuurnet/culturefeed/wiki/Tutorial-Search-page).

### Culturefeed_search_views

Views integration to list events, actors or productions. To create culturefeed views, create a new view and configure it to show Cdb items. Once the view is created, following handlers can be set:

- Fields
- Filters
- Sort

__[More info at wiki page](https://github.com/cultuurnet/culturefeed/wiki/Culturefeed-Search-UI)__

### Culturefeed_devel

This module logs every query to our API, on the screen for admins or to the watchdog. Handy tool to check which is the performance killer, your Drupal install or our API's.

### Culturefeed_entry_ui
Functions to do a CRUD on events are already in library, but we are building a form to create, update and delete events in our UiTdatabank as well.

__[More info at wiki page](https://github.com/cultuurnet/culturefeed/wiki/Culturefeed-Entry-UI)__

### Culturefeed_pages
User can create a page (locations, performers, families, schools, …) or become member and follower. 

__Wiki page not yet available__

### Culturefeed_ui
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

To integrate UiTID we also wrote a [tutorial](https://github.com/cultuurnet/culturefeed/wiki/Tutorial-CultureFeedUI).

### Culturefeed_social
All ‘social’ features: likes, comments, attends, etc.

__[More info at wiki page](https://github.com/cultuurnet/culturefeed/wiki/Culturefeed-Social)__

### Culturefeed_uitpas
Integration with the UiTPAS card system. It contains multiple pages and blocks that can be used to build an interface that allows:

- easy access to the advantages and promotions a passholder has access to,
- possibility to register your UiTPAS,
- highlights of promotions and actions.

__[More info at wiki page](https://github.com/cultuurnet/culturefeed/wiki/Culturefeed-UiTPAS)__

To integrate UiTPAS we wrote a [tutorial](https://github.com/cultuurnet/culturefeed/wiki/Tutorial-UiTPAS).

### Culturefeed_mailing
To create mailings with search results from event (based on lifestyleprofile, Vlieg weekendflash, …). __Not yet available for partners.__ Please contact us if interested. 

### Culturefeed_messages
Send messages to other users (most likely page owners), interesting when actors become pages.

### Culturefeed_userpoints_ui
Collect on line userpoints (for specific actions like writing reviews) and claim promotions (Vlieg).  __Not yet available for partners.__ Please contact us if interested. 

### Culturefeed_roles
Pre-assign roles to UiTID users that have not logged in yet. Drush integration.

### Culturefeed_calendar
UiTkalender functionality not yet released on UiTinVlaanderen (some extra features are going to be added in next release)

### Culturefeed_content
Adds a CultureFeed content field to add a search query to any of your content types.

### Culturefeed_saved_searches
With this functionality your user can save a search and create alerts from it.

### Culturefeed_sitemap
Creates a sitemap for events, productions and actors, see example http://www.uitinvlaanderen.be/sitemap.xml.

### Examples

There already a lot of integrations live on the 3.x version (and still counting):

- http://www.uitinvlaanderen.be
- http://www.uitinmechelen.be
- http://www.uitinsint-niklaas.be
- http://www.2014-18.be
- http://agenda.hbvl.be
- http://agenda.gva.be
- https://www.uitpas.be
- https://www.muntpunt.be/agenda/search
- http://www.starttoswim.com
- http://www.cultuurkuur.be
- http://www.uitmetvlieg.be
- http://agenda.besteburen.eu


## License

[Apache-2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
