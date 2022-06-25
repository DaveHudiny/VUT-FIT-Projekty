<x-layout>
    <br>
    <h1 class="ml-0 text-2xl font-bold text-black">Your page</h1>
    <br>
    <div>
        <h2 class="text-xl font-bold text-black">You are selling</h2>
        @if(Auth::user()->selling->first() != null)
            <div style="overflow-x:auto;">
                <table id="users">
                    <th>Auction ID</th>
                    <th>Auction name</th>
                    <th>You get/you pay</th>
                    <th>Approved</th>
                    <th>Finished</th>
                    @foreach(Auth::user()->selling as $auction)
                        @if($auction->type=="offer")
                            <tr>
                                <td>{{ $auction->id }}</td>
                                <td><a href="/auction/{{ $auction->id }}">{{ $auction->name }}</a></td>
                                <td>You get: 
                                    @if($auction->approved)
                                        <?php $offers = $auction->offer->where('approved', true);
                                        $offer = $offers->where("value", "=", $offers->max("value"));
                                        if($offer->first() != null){
                                            echo $offer->first()->value;
                                        } else {
                                            echo "No bid yet";    
                                        }
                                        ?>
                                    @else
                                        Auction not approved yet
                                    @endif
                                </td>
                                <td><?php if($auction->approved){ echo "Approved";} else{ echo "Not approved yet";} ?></td>
                                <td><?php if($auction->date < Carbon\Carbon::now()->toDateString()){ echo "Finished";} else{ echo "Ends: ".\Carbon\Carbon::parse($auction->date)->format('j F, Y');} ?></td>
                            </tr>
                        @endif
                    @endforeach
                </table>
            </div>
        @else
            No auctions yet!
        @endif
    </div>

    <div>
        <h2 class="text-xl font-bold text-black">You demand</h2>
        @if(Auth::user()->selling->first() != null)
            <div style="overflow-x:auto;">
                <table id="users">
                    <th>Auction ID</th>
                    <th>Auction name</th>
                    <th>You get/you pay</th>
                    <th>Approved</th>
                    <th>Finished</th>
                    @foreach(Auth::user()->selling as $auction)
                        @if($auction->type=="demand")
                            <tr>
                                <td>{{ $auction->id }}</td>
                                <td><a href="/auction/{{ $auction->id }}">{{ $auction->name }}</a></td>
                                <td>You pay:
                                    @if($auction->approved)
                                        <?php $demands = $auction->offer->where('approved', true);
                                            $demand = $demands->where("value", "=", $demands->min("value"));
                                            if($demand->first() != null){
                                                echo $demand->first()->value;
                                            } else {
                                                echo "No bid yet";    
                                            }
                                        ?>
                                    @else
                                        Auction not approved yet
                                    @endif
                                
                                </td>
                                <td><?php if($auction->approved){ echo "Approved";} else{ echo "Not approved yet";} ?></td>
                                <td><?php if($auction->date < Carbon\Carbon::now()->toDateString()){ echo "Finished";} else{ echo "Ends: ".\Carbon\Carbon::parse($auction->date)->format('j F, Y');} ?></td>
                            </tr>
                        @endif
                    @endforeach
                </table>
            </div>
        @else
            No auctions yet!
        @endif
    </div>

    <div>
        <h2 class="text-xl font-bold text-black">You have won:</h2>
        @if(($auctions = \App\Models\Auction::where("winner_id", "=", Auth::user()->id)->get()) != null)
            <div style="overflow-x:auto;">
                <table id="users">
                    <th>Auction ID</th>
                    <th>Auction name</th>
                    <th>Type</th>
                    <th>Price</th>
                    @foreach($auctions as $auction)
                            <tr>
                                <td>{{ $auction->id }}</td>
                                <td><a href="/auction/{{ $auction->id }}">{{ $auction->name }}</a></td>
                                <td>
                                    @if($auction->type=="demand")
                                        Demand
                                    @else
                                        Offer
                                    @endif
                                </td>
                                <td>
                                    @if($auction->type=="demand")
                                        {{ \App\Models\Offer::where("user_id", "=", Auth::user()->id)->first()->value }}
                                    @else
                                        Offer
                                    @endif
                                </td>
                            </tr>
                    @endforeach
                </table>
            </div>
        @else
            No auctions yet!
        @endif
    </div>

    <div>
        <h2 class="text-xl font-bold text-black">You are contested in:</h2>
        @if(($offers = \App\Models\Offer::where("user_id", "=", Auth::user()->id)->get()) != null)
            <div style="overflow-x:auto;">
                <table id="users">
                    <th>Auction ID</th>
                    <th>Auction name</th>
                    <th>Type</th>
                    <th>Price</th>
                    @foreach($offers as $offer)
                            <tr>
                                <td>{{ $offer->auction->id }}</td>
                                <td><a href="/auction/{{ $offer->auction->id }}">{{ $offer->auction->name }}</a></td>
                                <td>
                                    @if($offer->auction->type=="demand")
                                        Demand
                                    @else
                                        Offer
                                    @endif
                                </td>
                                <td>
                                    @if($auction->type=="demand")
                                        {{ $offer->value }} €
                                    @else
                                        Offer
                                    @endif
                                </td>
                            </tr>
                    @endforeach
                </table>
            </div>
        @else
            No auctions yet!
        @endif
    </div>


    <div>
        @if(Auth::user()->licitator)
            <h2 class="text-xl font-bold text-black">You are licitating</h2>
            @forelse (Auth::user()->licitating as $auction)
                <a href="/auction/{{ $auction->id }}"><li>{{ $auction->name }}</li></a>
                @if($auction->offer->first() == null)
                    <p>No offers yet</p>
                @else
                    <table id="users">
                        <th>User</th>
                        <th>Offer value</th>
                        <th>Approve</th>
                        <th>Delete offer</th>
                    @foreach(collect($auction->offer)->sortBy('value')->reverse() as $offer)
                        <tr>
                            <td><a href="/users/{{ $offer->user->id }}">{{ $offer->user->username }}</a></td>
                            <td>{{ $offer->value }}</td>
                            <td>
                                @if($offer->approved)
                                    Approved
                                @else
                                    <form method=POST action="/licitpage">
                                        @csrf
                                        <input hidden type="text" name="licitatorID" value={{ Auth::user()->id }}>
                                        <input hidden type="text" name="offerID" value={{ $offer->id }}>
                                        <input type="submit" name="action" value="Approve Offer">
                                    </form>
                                @endif
                            </td> 
                            <td>
                                @if($offer->approved)
                                    Approved
                                @else
                                    <form method=POST action="/licitpage">
                                        @csrf
                                        <input hidden type="text" name="licitatorID" value={{ Auth::user()->id }}>
                                        <input hidden type="text" name="offerID" value={{ $offer->id }}>
                                        <input type="submit" name="action" value="Refuse Offer">
                                    </form>
                                @endif
                            </td>
                        </tr>
                    @endforeach
                    </table>
                @endif
            @empty
                <p>No auctions yet!</p>
            @endforelse
        @endif
    </div>
</x-layout>
