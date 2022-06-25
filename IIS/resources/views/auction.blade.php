<x-layout>
    <div>
        <br>
        <h1 class="ml-2 text-2xl font-bold text-black">Auction ID: {{$auction->id}}</h1>
        <br>
    </div>
    <div class="m-auto md:grid grid-cols-1 lg:grid-cols-2 items-center">
        <div class="items-center bg-gray-100 dark:bg-gray-900 sm:items-center py-4 sm:pt-0">
            <h2 class="ml-2 text-lg font-bold text-black">{{$auction->name}}</h1>
            <div>
                Description: {{$auction->description}}
            </div>
            <div>
                End date (ends by 23:59:59): {{ \Carbon\Carbon::parse($auction->date)->format('j F, Y') }}
            </div>
            <div>
                Type of auction: {{$auction->type}}
            </div>
            <div>
                Auction rule: 
                @if($auction->is_open == 1)
                    open
                @else
                    closed
                @endif
            </div>
            <div>
                Starting price: {{$auction->starting_price}} €
            </div>
            <div>
                Minimal wager: {{$auction->minimum_wager}} €
            </div>
            <div>
                Special requirements: {{$auction->requirements}}
            </div>
            <div>
                    Actual price: <?php $offers = $auction->offer->where('approved', true);
                    if($auction->is_open==false AND $auction->confirmed==false)
                    {
                        echo "This auction is closed -- actual price will be released after finishing auction.";
                    }
                    else {
                        if($auction->type == "demand")
                        {
                            $offer = $offers->where("value", "=", $offers->min("value"));
                        }
                        else 
                        {
                            $offer = $offers->where("value", "=", $offers->max("value"));
                        }
                        if($offer->first() != null)
                        {
                            if($auction->type == "demand" and $offer->first()->value < $auction->starting_price)
                            {
                                echo $offer->first()->value." €";
                            }
                            elseif($auction->type == "offer" and $offer->first()->value > $auction->starting_price)
                            {
                                echo $offer->first()->value;
                            }
                            else {
                                echo $auction->starting_price." €";
                            }
                        }
                        else
                        {
                            echo $auction->starting_price;
                        }
                        echo "<br>";
                        if($auction->confirmed)
                        {
                            if($offer->first() != null)
                            {
                                $username = $offer->first()->user->username;
                                $id = $offer->first()->user->id;
                                echo "Winner is: <a href=\"/users/$id\">$username";
                            }
                        }
                    }
                    ?>
            </div>
            <div>
                Author: <a href="/users/{{$auction->user->id}}">{{$auction->user->username}}</a>
            </div>
            <div>
                @if($auction->date >= \Carbon\Carbon::now()->toDateString())
                    @if(Auth::check())
                        @if(Auth::user()->id == $auction->licitator_id)
                            You licitate this auction
                        @elseif($auction->approved AND \App\Models\Offer::where([['user_id', '=', Auth::user()->id], 
                            ['auction_id', '=', $auction->id], ['approved', false]])->first() == null)
                            @if($auction->approved AND \App\Models\Offer::where([['user_id', '=', Auth::user()->id], 
                                ['auction_id', '=', $auction->id], ['approved', true]])->first() != null AND $auction->is_open == false)
                                You already bidded in this closed auction.
                            @else
                                <form method=POST action="/auction/{{$auction->id}}">
                                    @csrf
                                    <input type="text" name="userID" value={{ Auth::user()->id }} hidden>
                                    <input type="text" name="auctionID" value={{ $auction->id }} hidden> 
                                    <input type="text" name="type" value={{ $auction->type }} hidden>
                                    <input type="checkbox" name="type" value={{ $auction->is_open}} hidden>
                                    <label>Your bid [€]:</label>
                                    <input type="number" name="bid" id="bid">
                                    @if(\App\Models\Offer::where([['user_id', '=', Auth::user()->id],['auction_id', '=', $auction->id]])->first() != null)
                                        <input type="submit" name="action" id="firstBid" value="Another bid">
                                    @else
                                        <input type="submit" name="action" id="firstBid" value="First bid">
                                    @endif    
                                </form>
                            @endif
                        @elseif($auction->approved AND \App\Models\Offer::where([['user_id', '=', Auth::user()->id], 
                        ['auction_id', '=', $auction->id], ['approved', false]])->first() != null)
                            Cannot create new bid before approval of old one.
                        @else
                            Auction not approved yet
                        @endif
                    @else
                        You have to register to bid!
                    @endif
                @else
                    Auction is finished.
                    @if($auction->ended)
                    <br>
                    Winner is: <?php $offers = $auction->offer->where('approved', true);
                    if($auction->type=="offer")
                    {
                        $offer = $offers->where("value", "=", $offers->max("value"));
                    }
                    else {
                        $offer = $offers->where("value", "=", $offers->min("value"));
                    }
                    if($offer->first() != null)
                    {
                        echo "Winner is: <a href=/users/".$offer->first()->user->id.">".$offer->first()->user->username."</a> with ".$offer->first()->value;
                    }
                    else
                    {
                        echo "Nobody contested";
                    }?>
                    @else
                    <br>
                    Auction is waiting for licitator to appove it.
                    @endif
                @endif
                @if ($errors->any())
                    <div class="alert alert-danger">
                        <ul>
                            @foreach ($errors->all() as $error)
                                <li>{{ $error }}</li>
                            @endforeach
                        </ul>
                    </div>
                @endif
            </div>
        </div>
        <div class="m-auto">
            @forelse ($auction->photo as $photo)
                @if ($loop->first)
                    <img src="{{ asset($auction->photo->first()->filename) }}" />
                @endif
            @empty
            @endforelse
        </div>
    </div>
    <br>
    <div class="m-auto">
        @forelse ($auction->photo as $photo)
            @if (! $loop->first)
                <img src="{{ asset($auction->photo->first()->filename) }}" />
            @endif
        @empty
            <h3>No images here</h3>
        @endforelse
    </div>
</x-layout>
