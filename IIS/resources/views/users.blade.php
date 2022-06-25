<x-layout>
    <div class="bg-gray-100 dark:bg-gray-900">
        <br>
        <h1 class="ml-0 text-2xl font-bold text-black">Users administration</h1>
        <br>
    </div>
    <div class="relative flex items-top justify-center min-h-screen bg-gray-100 dark:bg-gray-900 py-4 sm:pt-0">
        <div style="overflow-x:auto;">
            <table id="users">
                <tr>
                    <th>Username</th>
                    <th>Name</th>
                    <th>Email</th>
                    <th>Licitator</th>
                    <th>Promote/degrade</th>
                    <th>Delete user</th>
                </tr>
                @foreach($users as $user)
                <form action="/admin/users" method=POST>
                    <input type="text" name="username" value={{ $user->username }} hidden/>
                    @csrf
                    <tr>
                        <td>{{ $user->name }}</td>
                        <td><label type="text" name="username">{{ $user->username }}</label></td>
                        <td>{{ $user->email }}</td>
                        @if($user->licitator)
                            <td><input type="checkbox" name="licitator" onclick="return false;" checked/></td>
                            <td><input type="submit" value="Degrade" name="action"/></td>
                        @else
                            <td><input type="checkbox" name="licitator" onclick="return false;"/></td>
                            <td><input type="submit" value="Promote" name="action"/></td>
                        @endif
                        <td>
                        @if($user->username != "Admin")
                            <input type="submit" value="Delete" name="action" onclick="return confirm('Delete user?')"/>
                        @endif
                        </td>
                    </tr>
                </form>
                @endforeach
            </table>
        </div>
    </div>
</x-layout>