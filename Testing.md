# Manual Testing scenarious

Given: Nothing
When: /start -> "🆕Создать" -> [name of storage]
Then: New storage with name [name of storage] in the list of storages

Given: At least one existing storage with name [name of storage]
When: /start -> "🚮Удалить" -> [name of storage]
Then: Storage with name [name of storage] is removed from storage list 

Given: At least one storage with name [name of storage] and some ingredients 
When: /start -> [name of storage] -> "Продукты🍗"
Then: The list of ingredients in storage is showed

Given: At least one storage with name [name of storage]
When: /start -> [name of storage] -> "👥Участники"
Then: The list of members of [name of storage] is shown

Given: At least one storage with name [name of storage]
When: /start -> [name of storage] -> "👥Участники" -> "🔐Добавить" -> [forward message of needed user]
Then: New member is added to [name of storage], now he/she can operate with its ingredients 

Given: At least one storage with name [name of storage] and with at least one member with [name of member] in it
When: /start -> [name of storage] -> "👥Участники" -> "🔒Удалить" -> [name of member]
Then: Member [name of member] is deleted from [name of storage], has no access to it

Given: At least one storage with names [name of storage1], [name of storage2]..., at least one existing recipe in system, at least one existing ingredient in system 
When: /start -> "🤤Xочу кушать!" -> [name of storage1] -> [name of storage2] -> (until all storages which user wants to use are selected) -> "▶️Подтвердить"
Then: List of recipes with info about amount of ingredients which are present in recipe in chosen storages

Given: At least one storage with name [name of storage], at least one existing recipe in system, at least one existing ingredient in system and in [name of storage]
When: /start  -> [name of storage] -> "🤤Xочу кушать!"
Then: List of recipes with info about amount of ingredients which are present in recipe in chosen storages

Given: At least one storage with names [name of storage1], [name of storage2]..., at least one existing recipe in system, at least one existing ingredient in system
When: (/start -> "🤤Xочу кушать!" -> (choose storages) OR /start -> [name of storage] -> "🤤Xочу кушать!" -> [recipe name]
Then: The list of ingredients which are present/not present in chosen storages and ingredients marked with "?" Which means that user has storages where that ingredient is present but it's not chosen yet

Given: At least one storage with names [name of storage1], [name of storage2]..., at least one existing recipe in system, at least one existing ingredient in system and at least one storage which wasn't chosen but has ingredient which is not present in selected storages.
When: (/start -> "🤤Xочу кушать!" -> (choose storages) OR /start -> [name of storage] -> "🤤Xочу кушать!" -> [recipe name] -> "?"
Then: The list of storages which user may add to "chosen" storages to make some ingredients present
