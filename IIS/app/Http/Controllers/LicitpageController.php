<?php

namespace App\Http\Controllers;

use App\Models\User;
use App\Models\Offer;
use App\Models\Auction;
use App\Models\Photo;
use Illuminate\Http\Request;
use Carbon\Carbon;
use Illuminate\Support\Facades\Auth;

class LicitpageController extends Controller
{
    public function action(Request $request)
    {
        if ($request['action']=='Approve')
        {
            $auction=Auction::findOrFail($request['auctionID']);
            $auction->update(['approved' => true, 'licitator_id' => $request['licitatorID']]);

        }
        else if ($request['action']=='Refuse')
        {
            $auction=Auction::findOrFail($request['auctionID']);
            $photos=Photo::where('auction_id', '=', $auction['id'])->get();
            foreach ($photos as $photo)
            {
                unlink($photo['filename']);
            }
            $auction->delete();
        }
        else if ($request['action']=='End')
        {
            $auction=Auction::findOrFail($request['auctionID']);
            $auction->update(['ended' => true]);
        }
        else if ($request['action']=='Delete')
        {
            $auction=Auction::findOrFail($request['auctionID']);
            $auction->delete();
        }
        else if ($request['action']=='Confirm')
        {
            $auction=Auction::findOrFail($request['auctionID']);
            $auction->update(['confirmed' => true]);
        }
        // Nevím, jestli tlačítka na registrování uživatele do aukce chces mít na licitpage
        else if ($request['action']=='Approve Offer')
        {
            $user_id = $request['licitatorID'];
            $offer=Offer::findOrFail($request['offerID']);
            $offer->update(['approved' => true]);
            return redirect("/users/$user_id/auctions");
        }
        else if ($request['action']=='Refuse Offer')
        {
            $user_id = $request['licitatorID'];
            $offer=Offer::findOrFail($request['offerID']);
            $offer->delete();
            return redirect("/users/$user_id/auctions");
        }
        // Approve auction
        return view('licitpage', ['auctionUnapproved' => Auction::where([['approved', false], ['seller_id', '!=', Auth::user()->id]])->get(),
    'auctionApproved' => Auction::where([['approved', true],['date', '>=', Carbon::now()->toDateString()],
        ['licitator_id', '=', Auth::user()->id]])->get(),
    'auctionFinished' => Auction::where([['approved', true],['date', '<', Carbon::now()->toDateString()], 
        ['confirmed', false],['licitator_id', '=', Auth::user()->id]])->get(),
    'userRequests' => User::all()]);
    }
}
