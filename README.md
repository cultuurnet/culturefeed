# test #

# Backwards compatibility #

This section contains information regarding possible backwards compatibility breaks, and
outlines the necessary steps for upgrading third party code using the culturefeed module or
libraries contained in it.

## March 11, 2013 ##

### Custom HTTP proxy settings were replaced by Drupal core proxy settings ###

HTTP proxy support was added in Drupal 7.17, see http://drupal.org/node/7881 for more details.
CultureFeed from now on takes into account the HTTP proxy settings of Drupal core and longer uses
its own variables or settings form. To accomodate the proxy_exceptions setting of Drupal core,
DrupalCultureFeed::getOAuthClient() now requires the base URL of the webservice as a first argument.

## February 21, 2013 ##

### Classes prefixed with CultureFeed_Cdb moved out ###

All PHP classes prefixed with CultureFeed_Cdb were moved from the culturefeed module
to [their own repository][cultuurnet\cdb]. You need to install them separately, if you want
to use CultureFeed_EntryApi.

### Misplaced usage of CultureFeed\_Cdb_Item_Event::getExternalId() ###

CultureFeed_EntryApi::updateEvent(), CultureFeed_EntryApi::addTagToEvent() and
CultureFeed_EntryApi::removeTagFromEvent() now use the getCdbId() method on the
passed CultureFeed_Cdb_Item_Event instance. Previously it used getExternalId(),
which erroneously returned the CdbId instead of the external ID. When constructing
CultureFeed_Cdb_Item_Event instances in third party code, ensure the CdbId is set
with the setCdbId() method and exclusively use the setExternalId() method
for specifying an external ID.

[cultuurnet\cdb]: https://github.com/cultuurnet/cdb

## September 9, 2013 ##

hook_culturefeed_search_query_alter has been renamed to hook_culturefeed_search_page_query_alter.
The arguments have been changed to CultureFeedSearchPageInterface $culturefeedSearchPage.

Example of usage:
  // Add boost for relevance
  if (!empty($_GET['sort']) && $_GET['sort'] == 'relevancy') {
    $culturefeedSearchPage->setLocalParam('type', 'boost');
    $culturefeedSearchPage->setLocalParam('b', 'sum(recommend_count,product(comment_count,10))');
  }

