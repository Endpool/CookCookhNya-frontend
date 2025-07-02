# Manual Testing scenarious

Given: Nothing
When: /start -> + -> [name of storage]
Then: New storage with name [name of storage] in the list of storages

Given: At least one existing storage with name [name of storage]
When: /start -> "-" -> [name of storage]
Then: Storage with name [name of storage] is removed from storage list 

Given: At least one storage with name [name of storage] and some ingredients 
When: /start -> [name of storage] -> Исследовать
Then: The list of ingredients in storage is showed

Given: At least one storage with names [name of storage1], [name of storage2]..., at least one existing recipe in system, at least one existing ingredient in system 
When: /start -> хочу кушать -> [name of storage1] -> [name of storage2] -> (until all storages which user wants to use are selected) Исследовать
Then: The list of ingredients in storage is showed -> Подтвердить
Then: List of recipes with info about amount of ingredients which are present in recipe in chosen storages

Given: At least one storage with names [name of storage1], [name of storage2]..., at least one existing recipe in system, at least one existing ingredient in system
When: (/start -> хочу кушать -> (choose storages) OR /start -> [name of storageX] -> хочу кушать) -> [recipe name]
Then: The list of ingredients which are present/not present in chosen storages and ingredients marked with "?" Which means that user has storages where that ingredient is present but it's not chosen yet

Given: At least one storage with names [name of storage1], [name of storage2]..., at least one existing recipe in system, at least one existing ingredient in system and at least one storage which wasn't chosen but has ingredient which is not present in selected stoeages.
When: (/start -> хочу кушать -> (choose storages) OR /start -> [name of storageX] -> хочу кушать) -> [recipe name] -> "?"
Then: The list of storages which user may add to "chosen" storages to make some ingredients present
