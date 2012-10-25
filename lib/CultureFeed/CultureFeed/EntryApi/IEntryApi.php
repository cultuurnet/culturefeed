<?php

interface CultureFeed_EntryApi_IEntryApi {

  public function getEvent($id);

  public function createEvent(CultureFeed_Cdb_Event $event);

  public function updateEvent(CultureFeed_Cdb_Event $event);

  public function deleteEvent($id);

  public function addTagToEvent(CultureFeed_Cdb_Item_Event $event, $keywords);

  public function removeTagFromEvent(CultureFeed_Cdb_Item_Event $event, $keyword);

}