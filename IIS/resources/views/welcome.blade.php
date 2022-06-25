<x-layout>
    <!-- searchig and filtering -->
    <div class="justify-center pb-0 h-30 md:mt-4 sm:mt-1">
        <div class="relative sm:flex justify-center p-1 items-center">
            <form id="filter" method="GET" action="#" class="flex justify-center p-0 items-center">
                @if(request('type'))
                    <input type="hidden" name="type" value="{{ request('type') }}">
                @endif
                @if(request('rule'))
                    <input type="hidden" name="rule" value="{{ request('rule') }}">
                @endif
                <input type="text" name="search" placeholder="Search ..." class="w-64 h-full py-1 px-3 bg-white rounded-full text-lg font-bold placeholder-black" value="{{ request('search') }}">
            </form>
            <div class="flex justify-center p-0 items-center">
                <div x-data="{ show: false }" @click.away="show = false">
                    <button @click="show = ! show" class="my-1 py-2 px-3 w-24 h-full text-sm font-semibold text-lg font-bold leading-5 bg-white rounded-full">
                        {{ isset($currentType) ? ucwords($currentType) : 'Type' }}
                    </button>
                    <div x-show="show" class="absolute bg-white rounded-t-lg rounded-b-lg p-0 z-20 w-24" style="display:none">
                        <a href="/?{{ http_build_query(request()->except('type')) }}" class="block text-left hover:bg-blue-300 rounded-t-lg p-2">All</a>
                        <a href="/?type=demand&{{ http_build_query(request()->except('type')) }}" class="block text-left hover:bg-blue-300 p-2">Demand</a>
                        <a href="/?type=offer&{{ http_build_query(request()->except('type')) }}" class="block text-left hover:bg-blue-300 rounded-b-lg p-2">Offer</a>
                    </div>
                </div>

                <div x-data="{ show: false }" @click.away="show = false">
                    <button @click="show = ! show" class="my-1 py-2 px-3 w-24 h-full text-sm font-semibold text-lg font-bold leading-5 bg-white rounded-full">
                        {{ isset($currentRule) ? ucwords($currentRule) : 'Rule' }}
                    </button>
                    <div x-show="show" class="absolute bg-white rounded-t-lg rounded-b-lg p-0 z-20 w-24" style="display:none">
                        <a href="/?{{ http_build_query(request()->except('rule')) }}" class="block text-left hover:bg-blue-300 rounded-t-lg p-3">All</a>
                        <a href="/?rule=open&{{ http_build_query(request()->except('rule')) }}" class="block text-left hover:bg-blue-300 p-3">Open</a>
                        <a href="/?rule=closed&{{ http_build_query(request()->except('rule')) }}" class="block text-left hover:bg-blue-300 rounded-b-lg p-2">Closed</a>
                    </div>
                </div>
            </div>
        </div>
    </div>
    <!-- start of grid -->
    <div class="relative flex items-top justify-center min-h-screen bg-gray-100 sm:items-center pb-4 sm:pt-0">
        <div class="grid-container m-5 md:grid grid-cols-1 md:grid-cols-2 lg:grid-cols-4">
            @forelse($auctions as $auction)
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
                    if($offer->first() != null AND $auction->is_open==true)
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
    <div class="text-align:center">
        {{ $auctions->links() }}
    </div>
</x-layout>
