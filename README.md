# Backwards compatibility #

This section contains information regarding possible backwards compatibility breaks, and
outlines the necessary steps for upgrading third party code using the culturefeed module or
libraries contained in it.

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
