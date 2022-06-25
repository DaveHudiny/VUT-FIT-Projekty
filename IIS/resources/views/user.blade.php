<x-layout>
        <br>
        <h1 class="mx-3 text-2xl font-bold text-black">
            This is the profile of : {{$user->username}}
        </h1>
        <br>
        <br>
        <p>
            <h2 class="ml-10 text-xl font-bold text-black mb-2">If you want to contact me:</h2>
            <h3 class="ml-20 text-lg text-black mb-1">My name is:<b> {{$user->name}} </b></h3>
            <h3 class="ml-20 text-lg text-black">My email is:<b> {{$user->email}} </b></h3>
        </p>
        <br>
        <br>
        <h2 class="mx-5 text-xl font-bold text-black">
            I am an author of <b style='color:#04AA6D;'> {{$user->selling->count()}} </b> auctions:
        </h2>
        <div class="relative flex items-top justify-center min-h-0 bg-gray-100 dark:bg-gray-900 sm:items-center py-0">
            <div class="grid-container m-5 md:grid grid-cols-1 md:grid-cols-2 lg:grid-cols-4">
                @forelse ($user->selling as $auction)
                    <div class="grid-item">
                        <a href="/auction/{{$auction->id}}">
                            @if($auction->photo->first() != null) 
                                <img src="{{ asset($auction->photo->first()->filename) }}" />
                            @else
                                <img src="{{ asset("notfound.jpg") }}" />
                            @endif
                        </a>
                        <p><a href="/auction/{{$auction->id}}"><h3 class="text-bold text-lg">{{ $auction->name }}</h3></a></p>
                        <p>Author: <a href="/users/{{$auction->user->id}}">{{$auction->user->username}}</a></p>
                        <p>Ends: {{ \Carbon\Carbon::parse($auction->date)->format('j F, Y') }}</p>
                        <?php $offers = $auction->offer->where('approved', true);
                        if($auction->type="demand")
                        {
                            $offer = $offers->where("value", "=", $offers->min("value"));
                        }
                        else 
                        {
                            $offer = $offers->where("value", "=", $offers->max("value"));
                        }

                        if($offer->first() != null AND $auction->is_open==false)
                        {
                            echo "<p>Actual price: <b style='color:#04AA6D;'>".$offer->first()->value." €</b></p>";
                        }
                        else
                        {
                            echo "<p>Starting price: <b style='color:#04AA6D;'>".$auction->starting_price." €</b></p>";
                        }?>
                    </div>
                @empty
                    <h2>No auctions yet.</h2>
                @endforelse   
            </div>
        </div>
        <br>
        <br>
        <h2 class="mx-5 text-xl font-bold text-black">
            I currently licitate <b style='color:#04AA6D;'> {{$user->licitating->count()}} </b> auctions:
        </h2>
        <div class="relative flex items-top justify-center min-h-0 bg-gray-100 dark:bg-gray-900 sm:items-center py-0">
            <div class="grid-container m-5 md:grid grid-cols-1 md:grid-cols-2 lg:grid-cols-4">
            @forelse ($user->licitating as $auction)
                <div class="grid-item">
                    <a href="/auction/{{$auction->id}}">
                        @if($auction->photo->first() != null) 
                            <img src="{{ asset($auction->photo->first()->filename) }}" />
                        @else
                            <img src="{{ asset("notfound.jpg") }}" />
                        @endif
                    </a>
                    <p><a href="/auction/{{$auction->id}}"><h3 class="text-bold text-lg">{{ $auction->name }}</h3></a></p>
                    <p>Author: <a href="/users/{{$auction->user->id}}">{{$auction->user->username}}</a></p>
                    <p>Ends: {{ \Carbon\Carbon::parse($auction->date)->format('j F, Y') }}</p>
                    <?php $offers = $auction->offer->where('approved', true);
                    if($auction->type="demand")
                    {
                        $offer = $offers->where("value", "=", $offers->min("value"));
                    }
                    else 
                    {
                        $offer = $offers->where("value", "=", $offers->max("value"));
                    }

                    if($offer->first() != null AND $auction->is_open==false)
                    {
                        echo "<p>Actual price: <b style='color:#04AA6D;'>".$offer->first()->value." €</b></p>";
                    }
                    else
                    {
                        echo "<p>Starting price: <b style='color:#04AA6D;'>".$auction->starting_price." €</b></p>";
                    }?>
                </div>
                @empty
                    <h2>No auctions yet.</h2>
                @endforelse   
            </div>
        </div>
</x-layout>